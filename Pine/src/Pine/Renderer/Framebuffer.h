#pragma once

#include <vector>

namespace Pine {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		Framebuffer(const Framebuffer& other) = delete;
		virtual ~Framebuffer();

		void Invalidate();

		void Bind();
		void Unbind();
		bool IsBound() const { return m_IsBound; };

		void Resize(uint32_t width, uint32_t height);

		uint32_t GetColorAttachmentId(uint32_t index = 0);
		int32_t ReadRedPixel(uint32_t index, uint32_t x, uint32_t y);

		void ClearAttachment(uint32_t index, int32_t value);

		const FramebufferSpecification& GetSpecification() const { return m_Specification; }

	private:
		uint32_t m_FramebufferId;
		bool m_IsBound = false;

		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};

}
