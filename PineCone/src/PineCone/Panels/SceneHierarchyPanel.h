#pragma once

#include "Panel.h"
#include "PineCone\Core\SceneContext.h"
#include "PineCone\Controller\EditorCameraController.h"

#include <Pine.h>

#include <memory>

namespace Pine {

	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel(std::shared_ptr<SceneContext> context, std::shared_ptr<EditorCameraController> editorCamera)
			: m_SceneContext(context), m_EditorCamera(editorCamera) {};
		virtual ~SceneHierarchyPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "SceneHierarchyPanel"; };

	private:
		void DrawEntityNode(Entity entity);

	private:
		std::vector<std::function<void()>> m_EventListeners;

		std::shared_ptr<SceneContext> m_SceneContext;
		std::shared_ptr<EditorCameraController> m_EditorCamera;
		Entity m_SelectedEntity;
	};

}
