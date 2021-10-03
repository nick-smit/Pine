#include "ImGuiDemoPanel.h"

#include <imgui.h>

namespace Pine {

	ImGuiDemoPanel::ImGuiDemoPanel()
		: Panel("ImGui Demo Panel")
	{
	}

	ImGuiDemoPanel::~ImGuiDemoPanel()
	{
	}

	void ImGuiDemoPanel::OnRender(Timestep ts)
	{
		bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
	}

}