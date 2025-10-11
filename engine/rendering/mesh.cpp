module;
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fstream>
#include <glm/detail/func_geometric.inl>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iosfwd>
#include <iostream>
#include <optional>
#include <ostream>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <memory>
module nuit;

namespace Nuit
{
	bool MeshLoader::load(const std::string& filename)
	{
		reset();

		if (filename.ends_with(".obj"))
		{
			return load_obj(filename);
		}
		else
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(filename,
													 aiProcess_Triangulate | aiProcess_GenNormals |
														 aiProcess_JoinIdenticalVertices);


			if (!scene || !scene->HasMeshes())
			{
				std::println(std::cerr, "Failed to load file: {}", filename);
				return false;
			}

			std::println(std::cerr, "Unsupported file type: {}", filename);
		}
		return false;
	}

	void MeshLoader::draw(GLShaderProgram& shader) const
	{
		for (auto& mesh : m_meshes)
		{
			if (mesh.Material)
			{
				// If material textures were loaded correctly, we set the sampler2D id to the
				// textureID
				glActiveTexture(GL_TEXTURE0);
				if (mesh.Material->diffuseTex != 0)
				{
					glBindTexture(GL_TEXTURE_2D, mesh.Material->diffuseTex);
					shader.set_uniform("uDiffuseTex", 0);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, DummyTextureID);

					// Fallback color
					shader.set_uniform("uKd", mesh.Material->Kd);
				}

				glActiveTexture(GL_TEXTURE1);
				if (mesh.Material->ambientTex != 0)
				{
					glBindTexture(GL_TEXTURE_2D, mesh.Material->ambientTex);
					shader.set_uniform("uAmbientTex", 1);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, DummyTextureID);

					// Fallback color
					shader.set_uniform("uKa", mesh.Material->Ka);
				}

				// Set all other material-specific uniforms
				shader.set_uniform("uKs", mesh.Material->Ks);
				shader.set_uniform("uKe", mesh.Material->Ke);
				shader.set_uniform("uNs", mesh.Material->Ns);
				shader.set_uniform("uD", mesh.Material->d);
				shader.set_uniform("uIllum", mesh.Material->illum);
			}

			glBindVertexArray(mesh.Vao);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.Indices.size()), GL_UNSIGNED_INT,
						   nullptr);
		}
	}

	bool MeshLoader::load_obj(const std::string& filename)
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
							triangleVertices[j].Tangent = glm::normalize(
								triangleVertices[j].Tangent);
							triangleVertices[j].Bitangent = glm::normalize(
								triangleVertices[j].Bitangent);

							// Index where the vertex is stored
							uniqueVertices[triangleVertices[j]] = static_cast<uint32_t>(
								m_activeMesh->Vertices.size());

							// Push data afterward so we don't need to do size() - 1
							m_activeMesh->Vertices.push_back(triangleVertices[j]);
						}

						m_activeMesh->Indices.push_back(uniqueVertices[triangleVertices[j]]);
					}
				}
			}
			else if (token == "mtllib")
			{
				std::string prefix = filename.substr(0, filename.find_last_of('/')) + "/";
				iss >> token;

				load_mtllib(prefix + token);
			}
			else if (token == "usemtl")
			{
				std::string matName;
				iss >> matName;

				// Start a new mesh for this material
				m_meshes.push_back(Mesh{});
				m_activeMesh = &m_meshes.back();
				m_activeMesh->Material = m_materials[matName];
			}
		}

		set_attributes();

		return true;
	}

	void MeshLoader::load_mtllib(const std::string& filename)
	{
		std::ifstream file(filename);

		if (!file.is_open())
		{
			std::println(std::cerr, "Failed to open MTL file: {}", filename);
			return;
		}

		std::shared_ptr<Material> material;

		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::string token;
			iss >> token;

			if (token == "newmtl")
			{
				if (material)
				{
					m_materials[material->name] = material;
				}

				material = std::make_shared<Material>();

				iss >> material->name;
			}
			else if (token == "Ns")
			{
				iss >> material->Ns;
			}
			else if (token == "d")
			{
				iss >> material->d;
			}
			else if (token == "Tr")
			{
				// If Tr is set, we override d since they are the inverse of each other
				float tr;
				iss >> tr;
				material->d = 1.0f - tr;
			}
			else if (token == "Ka")
			{
				iss >> material->Ka.r >> material->Ka.g >> material->Ka.b;
			}
			else if (token == "Kd")
			{
				iss >> material->Kd.r >> material->Kd.g >> material->Kd.b;
			}
			else if (token == "Ks")
			{
				iss >> material->Ks.r >> material->Ks.g >> material->Ks.b;
			}
			else if (token == "Ke")
			{
				iss >> material->Ke.r >> material->Ke.g >> material->Ke.b;
			}
			else if (token == "illum")
			{
				iss >> material->illum;
			}
			else if (token == "map_Ka")
			{
				std::string tex;
				iss >> tex;
				material->map_Ka = tex;
			}
			else if (token == "map_Kd")
			{
				std::string tex;
				iss >> tex;
				material->map_Kd = tex;
			}
		}

		m_materials[material->name] = material;

		// Set prefix to use the same relative path as the .obj file
		std::string prefix = filename.substr(0, filename.find_last_of('/')) + "/";

		for (auto& mat : m_materials | std::views::values)
		{
			// Attempt to load textures, fallback to programmatically generated texture
			if (mat->map_Kd.has_value())
			{
				mat->diffuseTex = load_texture(prefix + mat->map_Kd.value());

				if (mat->diffuseTex == 0)
				{
					mat->diffuseTex = load_missing_texture();
				}
			}

			if (mat->map_Ka.has_value())
			{
				mat->ambientTex = load_texture(prefix + mat->map_Ka.value());

				if (mat->ambientTex == 0)
				{
					mat->ambientTex = load_missing_texture();
				}
			}
		}
	}

	void MeshLoader::reset()
	{
		m_meshes.clear();
		m_meshes.shrink_to_fit();
		m_activeMesh = nullptr;
		m_materials.clear();
	}

	void MeshLoader::set_attributes()
	{
		for (auto& mesh : m_meshes)
		{
			glGenVertexArrays(1, &mesh.Vao);
			glBindVertexArray(mesh.Vao);
			glGenBuffers(1, &mesh.Vbo);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.Vbo);
			glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex),
						 mesh.Vertices.data(), GL_STATIC_DRAW);

			glGenBuffers(1, &mesh.Ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.Ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(uint32_t),
						 mesh.Indices.data(), GL_STATIC_DRAW);

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
		}
	}
} // namespace Nuit
