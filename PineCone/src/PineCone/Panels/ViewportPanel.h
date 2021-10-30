#pragma once

#include "Panel.h"
#include "PineCone\Core\SceneContext.h"
#include "PineCone\Controller\EditorCameraController.h"

#include <Pine.h>

#include <memory>

#include <glm\glm.hpp>

namespace Pine {

	class ViewportPanel : public Panel {
	private:
		enum class GuizmoMode {
			Cursor, 
			Translate, 
			Rotate, 
			Scale 
		};

	public:
		ViewportPanel(std::shared_ptr<SceneContext> context, std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<EditorCameraController> camera)
			: m_Context(context), m_Framebuffer(framebuffer), m_Camera(camera) {};
		~ViewportPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;

		const glm::vec2& GetSize() const { return m_ViewportSize; }

	public:
		static std::string GetName() { return "ViewportPanel"; };

	private:
		void RenderToolbar();
		void RenderViewport();

	private:
		std::vector<std::function<void()>> m_EventListeners;
		
		std::shared_ptr<SceneContext> m_Context;
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<EditorCameraController> m_Camera;

		Entity m_SelectedEntity;

		glm::vec2 m_ViewportSize = { 0, 0 };
		glm::vec2 m_WindowSpaceMousePos = { -1, -1 };

		bool m_InFocus = false;
		bool m_IsHovered = false;

		GuizmoMode m_GuizmoMode = GuizmoMode::Cursor;
	};

}
