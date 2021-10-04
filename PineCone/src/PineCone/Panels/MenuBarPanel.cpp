#include "MenuBarPanel.h"

#include "ImGuiDemoPanel.h"

#include "PineCone\Core\PanelManager.h"

#include <Pine.h>

#include <imgui.h>

namespace Pine {
	void MenuBarPanel::OnAttach()
	{
		PINE_PROFILE_FUNCTION();

		EventDispatcher<KeyPressedEvent>::Listen([](const KeyPressedEvent& e) {
			switch (e.Key) {

			}

			return false;
		});
	}

	void MenuBarPanel::OnRender(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		PanelManager* panelManager = PanelManager::Get();
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Exit", "Alt+F4")) {
					Application::Get()->Close();
				}
		
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {

				#if PINE_DEBUG
				bool imguiDemoPanelActive = panelManager->IsPanelActive(ImGuiDemoPanel::GetName());
				if (ImGui::MenuItem("ImGui Demo", nullptr, &imguiDemoPanelActive)) {
					if (imguiDemoPanelActive)
						panelManager->ActivatePanel(ImGuiDemoPanel::GetName());
					else
						panelManager->DeactivatePanel(ImGuiDemoPanel::GetName());
				}
				#endif

				ImGui::EndMenu();
			}
		
			ImGui::EndMenuBar();
		}
	}

}