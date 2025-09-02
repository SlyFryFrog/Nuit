module;
#include <GL/glew.h>
#include <fstream>
#include <glm/detail/func_geometric.inl>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iosfwd>
#include <iostream>
#include <optional>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <vector>
module nuit;

namespace Nuit
{
	Mesh::~Mesh()
	{
		if (m_vao != 0)
		{
			glDeleteVertexArrays(1, &m_vao);
		}
		if (m_vbo != 0)
		{
			glDeleteBuffers(1, &m_vbo);
		}
		if (m_ebo != 0)
		{
			glDeleteBuffers(1, &m_ebo);
		}
	}

	bool Mesh::load(const std::string& filename)
	{
		reset_buffers();

		if (filename.ends_with(".obj"))
		{
			return load_obj(filename);
		}
		else
		{
			std::println(std::cerr, "Unsupported file type: {}", filename);
			return false;
		}
	}

	void Mesh::draw(const GLShaderProgram& shader) const
	{
		if (m_material)
		{
			// If material textures were loaded correctly, we set the sampler2D id to the textureID
			if (m_material->diffuseTex != 0)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_material->diffuseTex);
				shader.set_uniform("uDiffuseTex", 0);
			}
			else
			{
				// Fallback color
				shader.set_uniform("uKd", m_material->Kd);
			}

			if (m_material->ambientTex != 0)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_material->ambientTex);
				shader.set_uniform("uAmbientTex", 1);
			}
			else
			{
				// Fallback color
				shader.set_uniform("uKa", m_material->Ka);
			}

			// Set all other material-specific uniforms
			shader.set_uniform("uKs", m_material->Ks);
			shader.set_uniform("uKe", m_material->Ke);
			shader.set_uniform("uNs", m_material->Ns);
			shader.set_uniform("uD", m_material->d);
			shader.set_uniform("uIllum", m_material->illum);
		}

		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT,
					   nullptr);
		glBindVertexArray(0);
	}

	bool Mesh::load_obj(const std::string& filename)
	{
		std::ifstream file(filename);

		if (!file.is_open())
		{
			std::println(std::cerr, "Failed to open file: {}", filename);
			return false;
		}

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::unordered_map<Vertex, uint32_t, VertexHasher> uniqueVertices;

		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::string token;
			iss >> token;

			// Check the initial token (prefix) for the line
			if (token == "v")
			{
				glm::vec3 vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}
			else if (token == "vt")
			{
				glm::vec2 uv;
				iss >> uv.x >> uv.y;

				// Since we only care about 2D texcoords, we need to flip y
				uv.y = 1.0f - uv.y;
				uvs.push_back(uv);
			}
			else if (token == "vn")
			{
				glm::vec3 normal;
				iss >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (token == "f")
			{
				std::vector<std::string> faces;
				while (iss >> token)
				{
					faces.push_back(token);
				}

				// Iterate through each face
				// Loop 1: 0, 1, 2
				// Loop 2: 1, 2, 3
				for (size_t i = 1; i + 1 < faces.size(); i++)
				{
					const std::string& v0 = faces[0];
					const std::string& v1 = faces[i];
					const std::string& v2 = faces[i + 1];

					std::array verts{v0, v1, v2};
					std::array<Vertex, 3> triangleVertices;

					// Iterate through each vertex
					for (int j = 0; j < 3; j++)
					{
						std::istringstream vss(verts[j]);
						std::string posIdxStr, texIdxStr, normIdxStr;
						std::getline(vss, posIdxStr, '/');
						std::getline(vss, texIdxStr, '/');
						std::getline(vss, normIdxStr, '/');

						// Convert from start index 1 to 0
						int posIdx = std::stoi(posIdxStr) - 1;

						// Since tex and norms are optional, if not found, we set to -1
						int texIdx = !texIdxStr.empty() ? std::stoi(texIdxStr) - 1 : -1;
						int normIdx = !normIdxStr.empty() ? std::stoi(normIdxStr) - 1 : -1;

						// Create vertex and add it to vertices
						// May have duplicates which will be removed later
						Vertex vertex{};
						vertex.Position = vertices[posIdx];
						vertex.TexCoords = (texIdx >= 0 && texIdx < uvs.size()) ? uvs[texIdx]
																				: glm::vec2(0.0f);
						vertex.Normal = (normIdx >= 0 && normIdx < normals.size())
							? normals[normIdx]
							: glm::vec3(0.0f);

						triangleVertices[j] = vertex;
					}

					// Calculate edges and deltas relative to anchor vertex (position 0)
					glm::vec3 edge1 = triangleVertices[1].Position - triangleVertices[0].Position;
					glm::vec3 edge2 = triangleVertices[2].Position - triangleVertices[0].Position;
					glm::vec2 deltaUV1 = triangleVertices[1].TexCoords -
						triangleVertices[0].TexCoords;
					glm::vec2 deltaUV2 = triangleVertices[2].TexCoords -
						triangleVertices[0].TexCoords;

					// Calculate f, the inverse determinant
					float f = (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
					f = (f == 0.0f) ? 1.0f : 1.0f / f; // Avoid dividing by 0.0f

					glm::vec3 tangent = f * (edge1 * deltaUV2.y - edge2 * deltaUV1.y);
					glm::vec3 bitangent = f * (-edge1 * deltaUV2.x + edge2 * deltaUV1.x);

					for (int j = 0; j < 3; j++)
					{
						triangleVertices[j].Tangent += tangent;
						triangleVertices[j].Bitangent += bitangent;

						// Deduplicate vertices
						if (!uniqueVertices.contains(triangleVertices[j]))
						{
							// Index where the vertex is stored
							uniqueVertices[triangleVertices[j]] = static_cast<uint32_t>(
								m_vertices.size());

							// Push data afterward so we don't need to do size() - 1
							m_vertices.push_back(triangleVertices[j]);
						}

						m_indices.push_back(uniqueVertices[triangleVertices[j]]);
					}
				}
			}
			else if (token == "mtllib")
			{
				std::string prefix = filename.substr(0, filename.find_last_of('/')) + "/";
				iss >> token;

				if (auto mat = load_mtllib(prefix + token); mat.has_value())
				{
					m_material = std::make_shared<Material>(mat.value());
				}
			}
			else if (token == "usemtl")
			{
				// Do nothing for now as we don't support more than 1 material
			}
		}


		// Normalize tangents and bitangents
		for (auto& v : m_vertices)
		{
			v.Tangent = glm::normalize(v.Tangent);
			v.Bitangent = glm::normalize(v.Bitangent);
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(),
					 GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), m_indices.data(),
					 GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							  reinterpret_cast<void*>(offsetof(Vertex, Position)));
		glEnableVertexAttribArray(0);

		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							  reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glEnableVertexAttribArray(1);

		// TexCoords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							  reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
		glEnableVertexAttribArray(2);

		// Tangent
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							  reinterpret_cast<void*>(offsetof(Vertex, Tangent)));
		glEnableVertexAttribArray(3);

		// Bitangent
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							  reinterpret_cast<void*>(offsetof(Vertex, Bitangent)));
		glEnableVertexAttribArray(4);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}

	std::optional<Material> Mesh::load_mtllib(const std::string& filename)
	{
		Material material{};
		std::ifstream file(filename);

		if (!file.is_open())
		{
			std::println(std::cerr, "Failed to open MTL file: {}", filename);
			return std::nullopt;
		}

		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::string token;
			iss >> token;

			if (token == "newmtl")
			{
				iss >> material.name;
			}
			else if (token == "Ns")
			{
				iss >> material.Ns;
			}
			else if (token == "d")
			{
				iss >> material.d;
			}
			else if (token == "Tr")
			{
				// If Tr is set, we override d since they are the inverse of each other
				float tr;
				iss >> tr;
				material.d = 1.0f - tr;
			}
			else if (token == "Ka")
			{
				iss >> material.Ka.r >> material.Ka.g >> material.Ka.b;
			}
			else if (token == "Kd")
			{
				iss >> material.Kd.r >> material.Kd.g >> material.Kd.b;
			}
			else if (token == "Ks")
			{
				iss >> material.Ks.r >> material.Ks.g >> material.Ks.b;
			}
			else if (token == "Ke")
			{
				iss >> material.Ke.r >> material.Ke.g >> material.Ke.b;
			}
			else if (token == "illum")
			{
				iss >> material.illum;
			}
			else if (token == "map_Ka")
			{
				std::string tex;
				iss >> tex;
				material.map_Ka = tex;
			}
			else if (token == "map_Kd")
			{
				std::string tex;
				iss >> tex;
				material.map_Kd = tex;
			}
		}

		// Set prefix to use the same relative path as the .obj file
		std::string prefix = filename.substr(0, filename.find_last_of('/')) + "/";

		// Attempt to load textures, fallback to programmatically generated texture
		if (material.map_Kd.has_value())
		{
			material.diffuseTex = load_texture(prefix + material.map_Kd.value());
		}
		else
		{
			material.diffuseTex = load_missing_texture();
		}

		if (material.map_Ka.has_value())
		{
			material.ambientTex = load_texture(prefix + material.map_Ka.value());
		}
		else
		{
			material.ambientTex = load_missing_texture();
		}

		return material;
	}

	void Mesh::reset_buffers() const
	{
		if (m_vao != 0)
		{
			glDeleteVertexArrays(1, &m_vao);
		}
		if (m_vbo != 0)
		{
			glDeleteBuffers(1, &m_vbo);
		}
		if (m_ebo != 0)
		{
			glDeleteBuffers(1, &m_ebo);
		}
	}
} // namespace Nuit
