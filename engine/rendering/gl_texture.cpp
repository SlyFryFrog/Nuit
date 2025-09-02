module;
#include <GL/glew.h>
#include <iostream>
#include <print>
#include <stb_image.h>
module nuit;

namespace Nuit
{
	GLuint load_texture(const std::string& filename)
	{
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		// set wrapping/filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
						 nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::println(std::cerr, "Failed to load texture: {}", filename);
		}
		stbi_image_free(data);

		return texID;
	}

	GLuint load_missing_texture(const int width, const int height)
	{
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		std::vector<unsigned char> pixels(width * height * 3);

		// Set pixel values based on grid size
		// Will display an alternating black, purple pattern
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const int index = (y * width + x) * 3;		// Shift to start position
				const bool isPurple = (x + y) / 8 % 2 == 0; // Purple for every 8x8
				pixels[index + 0] = isPurple ? 255 : 0;
				pixels[index + 1] = 0;
				pixels[index + 2] = isPurple ? 255 : 0;
			}
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
					 pixels.data());

		return texID;
	}
} // namespace Nuit
