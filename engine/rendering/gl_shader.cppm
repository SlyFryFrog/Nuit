module;
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
export module nuit:gl_shader;

namespace Nuit
{
	export enum ShaderType {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		TESSELLATION = GL_TESS_CONTROL_SHADER,
		COMPUTE = GL_COMPUTE_SHADER
	};

	export class GLShaderProgram
	{
		GLuint m_program;

	public:
		GLShaderProgram() = default;
		~GLShaderProgram();

		/**
		 * @brief Returns the OpenGL ID of this shader program.
		 *
		 * @return The GLuint representing the program object.
		 */
		[[nodiscard]] GLuint id() const;

		/**
		 * @brief Creates a new shader id - should be called before
		 */
		void create();

		/**
		 * @brief Sets instance as the active shader.
		 */
		void bind() const;

		/**
		 * @brief Unbinds the active shader.
		 */
		static void unbind();

		/**
		 * @brief If you manually created a shader, you can link it to the program using this
		 * method.
		 *
		 * @param shader ID to shader instance.
		 */
		void attach_shader(GLuint shader) const;

		void compile_and_attach(const std::string& file, GLenum type) const;

		/**
		 * Links all attached shaders to the shader program.
		 */
		void link() const;

		[[nodiscard]] GLint get_uniform_location(const std::string& name) const;

		void set_uniform(const std::string& name, const int value) const
		{
			glUniform1i(get_uniform_location(name), value);
		}

		void set_uniform(const std::string& name, const glm::ivec2 value) const
		{
			glUniform2iv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::ivec3 value) const
		{
			glUniform3iv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::ivec4 value) const
		{
			glUniform4iv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const unsigned int value) const
		{
			glUniform1ui(get_uniform_location(name), value);
		}

		void set_uniform(const std::string& name, const glm::uvec2& value) const
		{
			glUniform2uiv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::uvec3& value) const
		{
			glUniform3uiv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::uvec4& value) const
		{
			glUniform4uiv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const float value) const
		{
			glUniform1f(get_uniform_location(name), value);
		}

		void set_uniform(const std::string& name, const glm::vec2& value) const
		{
			glUniform2fv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::vec3& value) const
		{
			glUniform3fv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::vec4& value) const
		{
			glUniform4fv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const double value) const
		{
			glUniform1d(get_uniform_location(name), value);
		}

		void set_uniform(const std::string& name, const glm::dvec2& value) const
		{
			glUniform2dv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::dvec3& value) const
		{
			glUniform3dv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::dvec4& value) const
		{
			glUniform4dv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::mat4& value) const
		{
			glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::mat3& value) const
		{
			glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::mat2& value) const
		{
			glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
		}
	};
} // namespace Nuit
