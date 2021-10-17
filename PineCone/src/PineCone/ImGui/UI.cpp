#include "pcpch.h"
#include "UI.h"

#include "ImGuiHelpers.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Pine {
	
	bool UI::DragFloat(const char* label, float* value, float speed, float min, float max, const char* format)
	{
		PC_IMGUI_BEGIN_INPUT_WIDGET_COLUMNS();

		ImGui::Text(label);
		ImGui::NextColumn();
		
		std::stringstream ss;
		ss << "##" << label;

		bool isUsing = ImGui::DragFloat(ss.str().c_str(), value, speed, min, max, format);

		PC_IMGUI_END_INPUT_WIDGET_COLUMNS();

		return isUsing;
	}

	void UI::Vec3Control(const char* label, glm::vec3& values, float resetValue)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label);

		PC_IMGUI_BEGIN_INPUT_WIDGET_COLUMNS();

		ImGui::Text(label);

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.17f, 0.6f, 0.17f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.7f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.17f, 0.6f, 0.17f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		PC_IMGUI_END_INPUT_WIDGET_COLUMNS();

		ImGui::PopID();
	}

	void UI::ColorEdit4(const char* label, glm::vec4& value)
	{
		PC_IMGUI_BEGIN_INPUT_WIDGET_COLUMNS();

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::ColorEdit4("", &value.r, ImGuiColorEditFlags_NoLabel);

		PC_IMGUI_END_INPUT_WIDGET_COLUMNS();
	}

	void UI::LabeledText(const char* label, const char* text)
	{
		PC_IMGUI_BEGIN_INPUT_WIDGET_COLUMNS();

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::Text(text);

		PC_IMGUI_END_INPUT_WIDGET_COLUMNS();
	}

}
