#include "pcpch.h"
#include "UI.h"

#include "ImGuiHelpers.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Pine {

	bool UI::s_InToolbar = false;

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
		auto boldFont = io.Fonts->Fonts[UI::GetFont(Font::Bold)];

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

		if (textWidth < maxWidth) {
			ImGui::SetCursorPosX((maxWidth - textWidth) * 0.5f + currentCursorPos.x);
			ImGui::Text(text.c_str());
		}
		else {
			// TODO: Fix centered text wrapping issues
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + maxWidth);
			ImGui::TextWrapped(text.c_str());
		}
	}

	bool UI::Button(const std::string& text, const glm::vec2& size, Status status)
	{
		auto& style = ImGui::GetStyle();

		ImVec4 originalColor = style.Colors[ImGuiCol_Button];
		ImVec4 activeColor = style.Colors[ImGuiCol_ButtonActive];

		switch (status) {
			case Status::Disabled:
				BeginDisabled();
				break;
			case Status::Active:
				style.Colors[ImGuiCol_Button] = activeColor;
				break;
		}

		bool clicked = ImGui::ButtonEx(text.c_str(), ImVec2(size.x, size.y));

		switch (status) {
			case Status::Disabled:
				EndDisabled();
				break;
			case Status::Active:
				style.Colors[ImGuiCol_Button] = originalColor;
				break;
		}

		return clicked;
	}

	bool UI::Button(const std::string& text, uint32_t size, Status status)
	{
		return Button(text, { (float) size, (float) size }, status);
	}

	bool UI::Button(const std::string& text, float size, Status status)
	{
		return Button(text, { size, size }, status);
	}

	bool UI::Button(UITexture texture, const glm::vec2& size, Status status)
	{
		auto& style = ImGui::GetStyle();
		auto* texLib = UITextureLibrary::Get();
		
		UITextureSize textureSize = UITextureSize::Small;
		if (std::max(size.x, size.y) > 96) {
			textureSize = UITextureSize::Large;
		}
		else if (std::max(size.x, size.y) > 48) {
			textureSize = UITextureSize::Medium;
		}

		switch (status) {
			case Status::Disabled:
				BeginDisabled();
				break;
			case Status::Active:
				ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
				break;
		}

		bool clicked = ImGui::ImageButton((void*)texLib->GetTextureID(texture), ImVec2(size.x, size.y), { 0,0 }, { 1,1 }, s_InToolbar ? 0: -1);

		switch (status) {
			case Status::Disabled:
				EndDisabled();
				break;
			case Status::Active:
				ImGui::PopStyleColor();
				break;
		}

		return clicked;
	}

	bool UI::Button(UITexture texture, uint32_t size, Status status)
	{
		return Button(texture, { (float)size, (float)size }, status);
	}

	bool UI::Button(UITexture texture, float size, Status status)
	{
		return Button(texture, { size, size }, status);
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

	void UI::BeginWindowWithToolbar(const std::string& name)
	{
		ImGuiWindowFlags viewportWindowFlags = 0
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoCollapse;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
		ImGui::Begin(name.c_str(), NULL, viewportWindowFlags);
		ImGui::PopStyleVar();
	}

	void UI::BeginWindowWithToolbar_Toolbar()
	{
		ImGuiWindowFlags childWindowFlags = 0
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_AlwaysUseWindowPadding;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 2, 2 });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 2, 2 });

		float toolbarHeight = GetToolbarButtonHeight() + ImGui::GetStyle().FramePadding.y * 2;
		ImGui::BeginChild("##toolbar", { 0, toolbarHeight }, false, childWindowFlags);
		
		ImGui::PopStyleVar();

		s_InToolbar = true;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		auto* window = ImGui::GetCurrentWindow();
		window->DC.LayoutType = ImGuiLayoutType_Horizontal;
	}

	void UI::EndWindowWithToolbar_Toolbar()
	{
		auto* window = ImGui::GetCurrentWindow();
		window->DC.LayoutType = ImGuiLayoutType_Vertical;

		s_InToolbar = false;
		ImGui::PopStyleColor();

		ImGui::PopStyleVar(); // Item spacing
		
		ImGui::EndChild();
	}

	void UI::BeginWindowWithToolbar_Content()
	{
		ImGuiWindowFlags childWindowFlags = 0
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_AlwaysUseWindowPadding;

		ImGui::BeginChild("##content", { 0,0 }, false, childWindowFlags);
	}

	void UI::EndWindowWithToolbar_Content()
	{
		ImGui::EndChild();
	}

	void UI::EndWindowWithToolbar()
	{
		ImGui::End();
	}

	void UI::ToolbarSeparator()
	{
		float currentSpacing = ImGui::GetStyle().ItemSpacing.x;
		float separatorSpacing = 4.0f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (separatorSpacing - currentSpacing));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { separatorSpacing, separatorSpacing });
		ImGui::Separator();
		ImGui::PopStyleVar();
	}

	float UI::GetToolbarButtonHeight()
	{
		return 20.0f;
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

	uint32_t UI::GetFont(Font font)
	{
		switch (font) {
			case Font::Regular: return 0;
			case Font::Italic: return 1;
			case Font::Bold: return 2;
			case Font::BoldItalic: return 3;
		}

		PINE_ASSERT(false, "Unknown font");
		return 0;
	}

	void UI::ApplyStyle()
	{
		auto fontBasePath = std::filesystem::current_path() / "editor_assets/font";
		auto& io = ImGui::GetIO();

		float fontsize = 16.0f;
		auto robotoRegular = fontBasePath / "roboto/Roboto-Regular.ttf";
		io.FontDefault = io.Fonts->AddFontFromFileTTF(robotoRegular.string().c_str(), fontsize);

		auto robotoItalic = fontBasePath / "roboto/Roboto-Italic.ttf";
		io.Fonts->AddFontFromFileTTF(robotoItalic.string().c_str(), fontsize);

		auto robotoBold = fontBasePath / "roboto/Roboto-Bold.ttf";
		io.Fonts->AddFontFromFileTTF(robotoBold.string().c_str(), fontsize);
		
		auto robotoBoldItalic = fontBasePath / "roboto/Roboto-BoldItalic.ttf";
		io.Fonts->AddFontFromFileTTF(robotoBoldItalic.string().c_str(), fontsize);

		auto& style = ImGui::GetStyle();

		style.TabRounding = 2.0f;
		style.WindowMenuButtonPosition = ImGuiDir_Right;

		ImVec4* colors = style.Colors;
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 0.00f, 0.89f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.91f, 0.00f, 1.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.35f, 0.36f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.91f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.30f, 0.36f, 0.46f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.36f, 0.42f, 0.53f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.48f, 0.58f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.23f, 0.33f, 0.47f, 0.61f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.39f, 0.54f, 0.61f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.47f, 0.61f, 0.61f);
		colors[ImGuiCol_Separator] = ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.23f, 0.33f, 0.47f, 0.61f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.28f, 0.39f, 0.54f, 0.61f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.00f, 1.00f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.00f, 1.00f, 0.20f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.91f, 0.00f, 1.00f, 0.20f);
		colors[ImGuiCol_Tab] = ImVec4(0.23f, 0.33f, 0.47f, 0.61f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.39f, 0.54f, 0.61f);
		colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.33f, 0.47f, 0.61f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.36f, 0.47f, 0.61f, 0.61f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.21f, 0.39f, 0.64f, 0.61f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.17f, 0.38f, 0.68f, 0.61f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

}
