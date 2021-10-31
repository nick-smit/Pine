#pragma once

#include "PineCone\ImGui\UITextureLibrary.h"

#include <Pine.h>

#include <memory>
#include <string>

#include <glm\glm.hpp>
#include <imgui.h>

namespace Pine {
	
	class UI {
	public:
		enum class Status { None, Default, Hovered, Active, Clicked, Disabled };
		enum class Font { Regular = 0, Italic = 1, Bold = 2, BoldItalic = 3, Default = Regular };
	public:
		static bool DragFloat(const char* label, float* value, float speed = 0.1f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f");

		static void Vec3Control(const char* label, glm::vec3& values, float resetValue = 0.0f);

		static void ColorEdit4(const char* label, glm::vec4& value);

		static void LabeledText(const char* label, const char* text);
		static void TextCentered(const std::string& text);
		static void TextCentered(const std::string& text, float maxWidth);

		static bool Button(const std::string& text, const glm::vec2& size = { 0, 0 }, Status status = Status::None);
		static bool Button(const std::string& text, uint32_t size, Status status = Status::None);
		static bool Button(const std::string& text, float size, Status status = Status::None);
		
		static bool Button(UITexture texture, const glm::vec2& size = { 0, 0 }, Status status = Status::None);
		static bool Button(UITexture texture, uint32_t size, Status status = Status::None);
		static bool Button(UITexture texture, float size, Status status = Status::None);

		static void BeginDisabled();
		static void EndDisabled();

		static void BeginWindowWithToolbar(const std::string& name);
		static void BeginWindowWithToolbar_Toolbar();
		static void EndWindowWithToolbar_Toolbar();
		static void BeginWindowWithToolbar_Content();
		static void EndWindowWithToolbar_Content();
		static void EndWindowWithToolbar();

		static void ToolbarSeparator();

		static float GetToolbarButtonHeight();

		static glm::vec2 GetWindowSpaceMousePosition();

		static ImVec4 GetColor(const std::string& name);
		static uint32_t GetFont(Font font);
		static void ApplyStyle();
	
	private:
		static bool s_InToolbar;
	};

}
