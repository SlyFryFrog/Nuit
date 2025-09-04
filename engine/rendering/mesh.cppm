module;
#include <GL/glew.h>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <string>
export module nuit:mesh;

import :gl_shader;
import :utils;

namespace Nuit
{
	struct Vertex
	{
		glm::vec3 Position{};
		glm::vec3 Normal{};
		glm::vec2 TexCoords{};
		glm::vec3 Tangent{};
		glm::vec3 Bitangent{};

		bool operator==(const Vertex& other) const
		{
			return Position == other.Position && Normal == other.Normal &&
				TexCoords == other.TexCoords;
		}
	};

	struct Material
	{
		std::string name;

		glm::vec3 Ka = {1.0f, 1.0f, 1.0f}; // Ambient color
		glm::vec3 Kd = {1.0f, 1.0f, 1.0f}; // Diffuse color
		glm::vec3 Ks = {0.0f, 0.0f, 0.0f}; // Specular color
		glm::vec3 Ke = {0.0f, 0.0f, 0.0f}; // Emissive color

		float Ns = 32.0f; // Shininess
		float d = 1.0f;	  // Opacity
		int illum = 2;	  // Illumination model

		std::optional<std::string> map_Ka; // Ambient texture
		std::optional<std::string> map_Kd; // Diffuse texture

		// OpenGL handles
		GLuint diffuseTex = 0;
		GLuint ambientTex = 0;
	};

	struct VertexHasher
	{
		std::size_t operator()(const Vertex& vertex) const
		{
			using std::hash;

			auto hashCombine = [](const size_t seed, const size_t v)
			{
				// Use 0x9e3779b9, the golden ratio constant
				return seed ^ (v + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			};

			size_t h = 0;

			// Position
			h = hashCombine(h, hash<float>()(vertex.Position.x));
			h = hashCombine(h, hash<float>()(vertex.Position.y));
			h = hashCombine(h, hash<float>()(vertex.Position.z));

			// Normal
			h = hashCombine(h, hash<float>()(vertex.Normal.x));
			h = hashCombine(h, hash<float>()(vertex.Normal.y));
			h = hashCombine(h, hash<float>()(vertex.Normal.z));

			// TexCoords
			h = hashCombine(h, hash<float>()(vertex.TexCoords.x));
			h = hashCombine(h, hash<float>()(vertex.TexCoords.y));

			// Tangent
			h = hashCombine(h, hash<float>()(vertex.Tangent.x));
			h = hashCombine(h, hash<float>()(vertex.Tangent.y));
			h = hashCombine(h, hash<float>()(vertex.Tangent.z));

			// Bitangent
			h = hashCombine(h, hash<float>()(vertex.Bitangent.x));
			h = hashCombine(h, hash<float>()(vertex.Bitangent.y));
			h = hashCombine(h, hash<float>()(vertex.Bitangent.z));

			return h;
		}
	};

	struct Mesh
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::shared_ptr<Material> Material;
		unsigned int MaterialIndex;
		GLuint m_vao{};
		GLuint m_vbo{};
		GLuint m_ebo{};
	};

	export class MeshLoader
	{
		std::shared_ptr<Material> m_material;
		std::vector<Mesh> m_meshes;
		std::unordered_map<std::string, std::shared_ptr<Material>, StringHasher> m_materials;
		Mesh* m_activeMesh = nullptr;

	public:
		MeshLoader() = default;

		~MeshLoader() = default;

		bool load(const std::string& filename);

		void draw(const GLShaderProgram& shader) const;

	private:
		bool load_obj(const std::string& filename);

		void load_mtllib(const std::string& filename);

		void reset();

		void set_attributes();
	};
} // namespace Nuit
