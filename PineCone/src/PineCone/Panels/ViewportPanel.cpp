#include "ViewportPanel.h"

#include <imgui.h>

namespace Pine {



	void ViewportPanel::OnAttach()
	{
	}

	void ViewportPanel::OnDetach()
	{
	}

	void ViewportPanel::OnRender(Timestep ts)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureId = m_Framebuffer->GetColorAttachmentId();
		ImGui::Image(reinterpret_cast<void*>(textureId), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();
	}

}