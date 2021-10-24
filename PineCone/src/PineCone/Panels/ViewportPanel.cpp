#include "pcpch.h"
#include "ViewportPanel.h"

#include "PineCone\Core\Event.h"
#include "PineCone\ImGui\ErrorPopup.h"

#include <imgui.h>

#include <string>

namespace Pine {

	void ViewportPanel::OnRender(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureId = m_Framebuffer->GetColorAttachmentId();
		ImGui::Image(reinterpret_cast<void*>(textureId), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0)); 
		
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file")) {
				const std::filesystem::path path = *(const std::filesystem::path*)payload->Data;

				if (path.extension().string() == ".pinescene") {
					EventDispatcher<SceneOpenedEvent>::Dispatch({ path });
				}
				else {
					ErrorPopup::Open(fmt::format("Cannot open '{0}' in the viewport", path.filename().string()));
				}
			}

			ImGui::EndDragDropTarget();
		}

		bool oldFocusStatus = m_InFocus;
		m_InFocus = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();
		if (m_InFocus != oldFocusStatus) {
			EventDispatcher<ViewportFocusedEvent>::Dispatch({ m_InFocus });
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

}