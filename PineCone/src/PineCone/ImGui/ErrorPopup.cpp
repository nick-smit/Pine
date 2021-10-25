#include "pcpch.h"
#include "ErrorPopup.h"

#include "UI.h"

namespace Pine {
	
	bool ErrorPopup::s_IsOpen = false;
	std::string ErrorPopup::s_Reason{};

	void ErrorPopup::Render()
	{
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (s_IsOpen && !ImGui::IsPopupOpen("Error"))
			ImGui::OpenPopup("Error");

		if (ImGui::BeginPopupModal("Error", &s_IsOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 350.0f);
			ImGui::Text(s_Reason.c_str());

			ImGui::NewLine();

			ImGui::SetItemDefaultFocus();
			if (UI::Button("OK", { 120, 0 })) {
				s_IsOpen = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void ErrorPopup::Open(const std::string& reason)
	{
		s_Reason = reason;
		s_IsOpen = true;
	}

}
