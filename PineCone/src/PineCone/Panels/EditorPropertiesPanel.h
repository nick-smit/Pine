#pragma once
#include "Panel.h"

#include "PineCone\Controller\EditorCameraController.h"
#include "PineCone\Core\SceneContext.h"

namespace Pine {

	class EditorPropertiesPanel : public Panel {
	public:
		EditorPropertiesPanel(std::shared_ptr<EditorCameraController> editorCamera)
			: m_EditorCamera(editorCamera) {};

		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "EditorPropertiesPanel"; };

	private:
		std::shared_ptr<EditorCameraController> m_EditorCamera;
	};

}
