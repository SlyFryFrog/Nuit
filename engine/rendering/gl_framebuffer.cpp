module;
#include <GL/glew.h>
#include <iostream>
#include <print>
module nuit;

namespace Nuit
{
	GLFramebuffer::GLFramebuffer() = default;

	GLFramebuffer::GLFramebuffer(const int width, const int height) :
		m_width(width), m_height(height)
	{
	}

	GLFramebuffer::~GLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_fbo);
		glDeleteRenderbuffers(1, &m_rbo);
		glDeleteTextures(1, &m_texID);
	}

	void GLFramebuffer::create()
	{
		// Generate and bind framebuffer
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// Create and attach color texture
		m_texID = create_texture(m_width, m_height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texID, 0);

		// Create renderbuffer for depth/stencil
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
								  m_rbo);

		// Check completeness
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::println(std::cerr, "Framebuffer not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFramebuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void GLFramebuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void GLFramebuffer::update(const int width, const int height)
	{
		m_width = width;
		m_height = height;
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
					 nullptr);

		// Create a buffer storage
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
} // namespace Nuit
