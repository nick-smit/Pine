#include "pinepch.h"
#include "Texture2D.h"

#include <glad\glad.h>
#include <stb\stb_image.h>

namespace Pine {

	Texture2D::Texture2D(const Specification& spec, const std::string& path)
		: m_Spec(spec)
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		PINE_ASSERT(data, "Failed to load {0}", path);

		m_Width = (uint32_t)width;
		m_Height = (uint32_t)height;
		m_Channels = (uint32_t)channels;

		if (data) {
			Load(data);

			stbi_image_free(data);
		}
	}

	Texture2D::Texture2D(const Specification& spec, uint32_t width, uint32_t height, uint32_t channels, void* data, size_t size)
		: m_Spec(spec), m_Width(width), m_Height(height), m_Channels(channels)
	{
		PINE_ASSERT((size == m_Channels * m_Width * m_Height), "Supplied an incomplete texture!");
		Load(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_TextureId);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		PINE_ASSERT((slot < 32), "Pine does not allow more than 16 textures!");

		glBindTextureUnit(slot, m_TextureId);
	}

	void Texture2D::Load(void* data)
	{
		GLenum internalFormat = m_Channels == 3 ? GL_RGB8 : GL_RGBA8;
		GLenum dataFormat = m_Channels == 3 ? GL_RGB : GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
		glTextureStorage2D(m_TextureId, 1, internalFormat, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

}
