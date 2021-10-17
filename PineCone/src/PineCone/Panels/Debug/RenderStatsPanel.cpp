#include "pcpch.h"
#include "RenderStatsPanel.h"

#include "PineCone\ImGui\UI.h"

#include <Pine.h>

#include <imgui.h>

namespace Pine {
	
	void RenderStatsPanel::OnRender(Timestep ts)
	{
		ImGui::Begin("Render statistics");

		UI::LabeledText("FPS:", std::to_string(1.0f / ts).c_str());
		UI::LabeledText("Frametime:", std::to_string(ts).c_str());

		ImGui::Separator();

		const Renderer2D::Stats stats = Renderer2D::GetStats();

		ImGui::Text("Renderer 2D statistics:");

		UI::LabeledText("Draw calls:", std::to_string(stats.DrawCalls).c_str());
		UI::LabeledText("Batches:", std::to_string(stats.Batches).c_str());
		UI::LabeledText("Quads", std::to_string(stats.Quads).c_str());

		ImGui::End();
	}

}
