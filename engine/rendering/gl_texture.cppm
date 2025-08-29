module;
#include <GL/glew.h>
#include <vector>
export module nuit:gl_texture;

namespace Nuit
{
	export class GLTexture
	{
		GLuint m_id{};
		int m_width, m_height;

	public:
		GLTexture(const int width, const int height) : m_width(width), m_height(height)
		{
		}

		[[nodiscard]] GLuint id() const
		{
			return m_id;
		}

		void create_missing_texture(const int sizeX = 32, const int sizeY = 32)
		{
			glGenTextures(1, &m_id);
			glBindTexture(GL_TEXTURE_2D, m_id);

			std::vector<unsigned char> pixels(sizeX * sizeY * 3);

			// Set pixel values based on grid size
			// Will display an alternating black, purple pattern
			for (int y = 0; y < sizeY; ++y)
			{
				for (int x = 0; x < sizeX; ++x)
				{
					const int index = (y * sizeX + x) * 3;		// Shift to start position
					const bool isPurple = (x + y) / 8 % 2 == 0;	// Purple for every 8x8
					pixels[index + 0] = isPurple ? 255 : 0;
					pixels[index + 1] = 0;
					pixels[index + 2] = isPurple ? 255 : 0;
				}
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
		}
	};

	export class GLTextureManager
	{
	public:
	};
} // namespace Nuit
