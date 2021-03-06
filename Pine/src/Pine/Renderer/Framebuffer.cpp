#include "pinepch.h"

#include <glad\glad.h>
#include "Framebuffer.h"

namespace Pine {

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils {
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			PINE_PROFILE_FUNCTION();

			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			PINE_PROFILE_FUNCTION();

			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			PINE_PROFILE_FUNCTION();

			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

		static GLenum PineFramebufferTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
				case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			PINE_ASSERT(false, "Unknown framebuffer texture format.");
			return 0;
		}
	}

	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		PINE_PROFILE_FUNCTION();

		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat)) {
				m_ColorAttachmentSpecifications.emplace_back(spec);
			}
			else {
				m_DepthAttachmentSpecification = spec;
			}
		}

		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		PINE_PROFILE_FUNCTION();

		glDeleteFramebuffers(1, &m_FramebufferId);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void Framebuffer::Invalidate()
	{
		PINE_PROFILE_FUNCTION();

		if (m_FramebufferId) {
			glDeleteFramebuffers(1, &m_FramebufferId);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_FramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);

		bool multisample = m_Specification.Samples > 1;

		{
			PINE_PROFILE_SCOPE("Pine::Framebuffer::Invalidate - Setup Color Attachments");

			if (m_ColorAttachmentSpecifications.size()) {
				m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
				Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

				for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
					Utils::BindTexture(multisample, m_ColorAttachments[i]);

					switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
						case FramebufferTextureFormat::RGBA8:
							Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
							break;
						case FramebufferTextureFormat::RED_INTEGER:
							Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
							break;
						default: PINE_ASSERT(false, "Unknown textureformat");
					}
				}
			}
		}

		{
			PINE_PROFILE_SCOPE("Pine::Framebuffer::Invalidate - Setup Depth Attachments");
			if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
				Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
				Utils::BindTexture(multisample, m_DepthAttachment);
				switch (m_DepthAttachmentSpecification.TextureFormat)
				{
					case FramebufferTextureFormat::DEPTH24STENCIL8:
						Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
						break;
					default: PINE_ASSERT(false, "Unknown textureformat");
				}
			}
		}

		if (m_ColorAttachments.size() > 1) {
			PINE_ASSERT(m_ColorAttachments.size() <= 4, "Unable to draw to more than 4 buffers");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty()) {
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		PINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete.");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind()
	{
		if (m_IsBound) {
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
		m_IsBound = true;
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_IsBound = false;
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		PINE_PROFILE_FUNCTION();

		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			PINE_LOG_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	uint32_t Framebuffer::GetColorAttachmentId(uint32_t index)
	{
		PINE_ASSERT(index < m_ColorAttachments.size(), "Index out of bounds");
		
		return m_ColorAttachments[index];
	}

	int32_t Framebuffer::ReadRedPixel(uint32_t index, uint32_t x, uint32_t y)
	{
		PINE_PROFILE_FUNCTION();

		PINE_ASSERT(index < m_ColorAttachments.size(), "Index out of bounds");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);

		int data;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);

		return data;
	}

	void Framebuffer::ClearAttachment(uint32_t index, int32_t value)
	{
		PINE_PROFILE_FUNCTION();

		PINE_ASSERT(index < m_ColorAttachments.size(), "Index out of bounds");

		auto& spec = m_ColorAttachmentSpecifications[index];
		glClearTexImage(m_ColorAttachments[index], 0, Utils::PineFramebufferTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}

}