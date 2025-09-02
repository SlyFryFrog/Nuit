module;
#include <GL/glew.h>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
export module nuit:mesh;

import :gl_shader;

namespace Nuit
{
	export struct Vertex
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

	export class Mesh
	{
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		GLuint m_vao{};
		GLuint m_vbo{};
		GLuint m_ebo{};

	public:
		Mesh() = default;

		~Mesh();

		bool load(const std::string& filename);

		void draw() const;

	private:
		bool load_obj(const std::string& filename);

		void reset_buffers() const;
	};
} // namespace Nuit
