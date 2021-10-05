#include "ImGuiDemoPanel.h"

#include "PineCone\Core\PanelManager.h"

#include <imgui.h>

namespace Pine {

	void ImGuiDemoPanel::OnRender(Timestep ts)
	{
		bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);

		if (!show_demo_window) {
			PanelManager::Get()->DeactivatePanel(GetName());
		}
	}

}
