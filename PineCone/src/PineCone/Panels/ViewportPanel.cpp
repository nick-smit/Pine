#include "ViewportPanel.h"

#include <imgui.h>

namespace Pine {

	void ViewportPanel::OnAttach()
	{
		Texture2D::Specification spec;
		m_Texture = std::make_unique<Texture2D>(spec, "./assets/textures/checkerboard.png");
	}

	void ViewportPanel::OnDetach()
	{
	}

	void ViewportPanel::OnRender(Timestep ts)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		//auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		//auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		//auto viewportOffset = ImGui::GetWindowPos();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		uint32_t textureId = m_Texture->GetId();
		ImGui::Image(reinterpret_cast<void*>(textureId), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();
	}

}