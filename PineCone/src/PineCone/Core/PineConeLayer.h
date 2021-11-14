#include <Pine.h>

#include "PineCone\Core\SceneContext.h"
#include "PineCone\Core\PanelManager.h"
#include "PineCone\Command\CommandManager.h"
#include "PineCone\Panels\Panel.h"

#include "PineCone\Controller\EditorCameraController.h"

#include "PineCone\ImGui\UITextureLibrary.h"

#include <Pine.h>

#include <functional>
#include <memory>
#include <vector>

namespace Pine {

	class PineConeLayer : public Layer
	{
	public:
		PineConeLayer();
		virtual ~PineConeLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;

	private:
		void BeginDockspace() const;
		void EndDockspace() const;

		void UpdateScene(Timestep ts);
	private:
		std::vector<std::function<void()>> m_UnsubscribeFunctions;

		PanelManager m_PanelManager;
		CommandManager m_CommandManager;

		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<EditorCameraController> m_CameraController;

		std::shared_ptr<Scene> m_EditorScene = nullptr;
		std::shared_ptr<SceneContext> m_SceneContext = nullptr;

		std::unique_ptr<UITextureLibrary> m_UITextureLibrary = nullptr;


		bool m_ViewportInFocus = false;
	};

}
