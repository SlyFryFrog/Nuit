module;
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>
export module nuit:gl_shader;

import :utils;

namespace Nuit
{
	/**
	 * @brief OpenGL shader types (requires OpenGL 4.1+ for full support).
	 */
	export enum ShaderType {
		VERTEX = GL_VERTEX_SHADER,			   ///< Vertex shader (OpenGL 2.0+)
		FRAGMENT = GL_FRAGMENT_SHADER,		   ///< Fragment shader (OpenGL 2.0+)
		GEOMETRY = GL_GEOMETRY_SHADER,		   ///< Geometry shader (OpenGL 3.2+)
		TESSELLATION = GL_TESS_CONTROL_SHADER, ///< Tessellation control shader (OpenGL 4.0+)
		COMPUTE = GL_COMPUTE_SHADER			   ///< Compute shader (OpenGL 4.3+)
	};

	export class GLShaderProgram
	{
		GLuint m_program{0};
		std::unordered_map<std::string, GLint, StringHasher> m_uniformsCache;

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

		/**
		 * @brief Compiles a given .glsl shader and attaches it to the shader program.
		 *
		 * @param file Shader file to be read.
		 * @param type Type of shader (Vertex, Fragment, etc.).
		 */
		void compile_and_attach(const std::string& file, ShaderType type) const;

		/**
		 * @brief Compiles a given string of code and attaches it to the shader program.
		 *
		 * @note This function does not read a file, only takes in a string containing .glsl code.
		 *
		 * @param contents .glsl shader contents.
		 * @param type Type of shader (Vertex, Fragment, etc.).
		 */
		void compile_and_attach_contents(const std::string& contents, ShaderType type) const;

		/**
		 * @brief Links all attached shaders to the shader program.
		 */
		void link();

		/**
		 * @brief Returns the location of a given uniform in the currently active shader.
		 *
		 * @param name Uniform name defined in .glsl shader
		 * @return The location of the uniform.
		 */
		[[nodiscard]] GLint get_uniform_location(const std::string& name);

		void set_uniform(const std::string& name, const int value)
		{
			glUniform1i(get_uniform_location(name), value);
		}

		void set_uniform(const std::string& name, const glm::ivec2 value)
		{
			glUniform2iv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::ivec3 value)
		{
			glUniform3iv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::ivec4 value)
		{
			glUniform4iv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const unsigned int value)
		{
			glUniform1ui(get_uniform_location(name), value);
		}

		void set_uniform(const std::string& name, const glm::uvec2& value)
		{
			glUniform2uiv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::uvec3& value)
		{
			glUniform3uiv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::uvec4& value)
		{
			glUniform4uiv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const float value)
		{
			glUniform1f(get_uniform_location(name), value);
		}

		void set_uniform(const std::string& name, const glm::vec2& value)
		{
			glUniform2fv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::vec3& value)
		{
			glUniform3fv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::vec4& value)
		{
			glUniform4fv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const double value)
		{
			glUniform1d(get_uniform_location(name), value);
		}

		void set_uniform(const std::string& name, const glm::dvec2& value)
		{
			glUniform2dv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::dvec3& value)
		{
			glUniform3dv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::dvec4& value)
		{
			glUniform4dv(get_uniform_location(name), 1, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::mat4& value)
		{
			glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::mat3& value)
		{
			glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		void set_uniform(const std::string& name, const glm::mat2& value)
		{
			glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
		}
	};
} // namespace Nuit
