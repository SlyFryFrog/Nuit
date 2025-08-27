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
		explicit GLShaderProgram() = default;
		~GLShaderProgram();

		[[nodiscard]] GLuint id() const;

		void create();

		void bind() const;

		static void unbind();

		void attach_shader(GLuint shader) const;

		void compile_and_attach(const std::string& file, GLenum type) const;

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
	};
} // namespace Nuit
