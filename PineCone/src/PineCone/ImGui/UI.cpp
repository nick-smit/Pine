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

	void UI::TextCentered(const std::string& text)
	{
		float windowWidth = ImGui::GetWindowSize().x;

		UI::TextCentered(text, windowWidth);
	}

	void UI::TextCentered(const std::string& text, float maxWidth)
	{
		ImVec2 currentCursorPos = ImGui::GetCursorPos();

		float textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((maxWidth - textWidth) * 0.5f + currentCursorPos.x);
		ImGui::Text(text.c_str());
	}

	bool UI::Button(const std::string& text, const glm::vec2& size, bool disabled)
	{
		if (disabled)
			BeginDisabled();

		bool clicked = ImGui::Button(text.c_str(), ImVec2(size.x, size.y));
		
		if (disabled)
			EndDisabled();

		return clicked;
	}

	bool UI::ImageButton(UITexture texture, const glm::vec2& size, bool disabled)
	{
		auto* textureLib = UITextureLibrary::Get();

		return ImageButton(textureLib->GetTexture(texture), size, disabled);
	}

	bool UI::ImageButton(std::shared_ptr<Pine::Texture2D> texture, const glm::vec2& size, bool disabled)
	{
		if (disabled)
			BeginDisabled();

		bool clicked = ImGui::ImageButton((void*)texture->GetId(), ImVec2(size.x, size.y));

		if (disabled)
			EndDisabled();

		return clicked;
	}

	void UI::BeginDisabled()
	{
		auto& style = ImGui::GetStyle();

		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * 0.5f);
	}

	void UI::EndDisabled()
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	glm::vec2 UI::GetWindowSpaceMousePosition()
	{
		if (!ImGui::IsWindowHovered())
			return { -1,-1 };

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;

		ImVec2 screenSpaceMousePos = ImGui::GetMousePos();

		return { screenSpaceMousePos.x - vMin.x, screenSpaceMousePos.y - vMin.y };
	}

	ImVec4 UI::GetColor(const std::string& name)
	{
		if (name == "ActionBarBg") return ImVec4(0.266f, 0.316f, 0.340f, 1.0f); // rgb: 68, 81, 87
		
		if (name == "Icon_Default") return ImVec4(0.8f, 0.8f, 0.8f, 1);

		PINE_ASSERT("Undefined color: '{0}'!", name);
		return ImVec4(1.0f, 0.0f, 0.898f, 1.0f);
	}

}
