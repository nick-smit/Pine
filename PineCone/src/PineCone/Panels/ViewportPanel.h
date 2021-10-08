#pragma once

#include "Panel.h"

#include <Pine.h>

#include <memory>

#include <glm\glm.hpp>

namespace Pine {

	class ViewportPanel : public Panel {
	public:
		ViewportPanel(std::shared_ptr<Framebuffer> framebuffer)
			: m_Framebuffer(framebuffer) {};
		~ViewportPanel() = default;

		virtual void OnRender(Timestep ts) override;

		const glm::vec2& GetSize() const { return m_ViewportSize; }

	public:
		static std::string GetName() { return "ViewportPanel"; };

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = { 0, 0 };
	};

}
