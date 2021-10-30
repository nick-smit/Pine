#pragma once

#include "Panel.h"
#include "PineCone\Core\SceneContext.h"

#include <Pine.h>

#include <memory>

#include <glm\glm.hpp>

namespace Pine {

	class ViewportPanel : public Panel {
	public:
		ViewportPanel(std::shared_ptr<SceneContext> context, std::shared_ptr<Framebuffer> framebuffer)
			: m_Context(context), m_Framebuffer(framebuffer) {};
		~ViewportPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;

		const glm::vec2& GetSize() const { return m_ViewportSize; }

	public:
		static std::string GetName() { return "ViewportPanel"; };

	private:
		std::vector<std::function<void()>> m_EventListeners;
		
		std::shared_ptr<SceneContext> m_Context;
		std::shared_ptr<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = { 0, 0 };
		glm::vec2 m_WindowSpaceMousePos = { -1, -1 };

		bool m_InFocus = false;
		bool m_IsHovered = false;
	};

}
