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
		static bool DragFloat(const char* label, float* value, float speed = 0.1f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f");

		static void Vec3Control(const char* label, glm::vec3& values, float resetValue = 0.0f);

		static void ColorEdit4(const char* label, glm::vec4& value);

		static void LabeledText(const char* label, const char* text);
		static void TextCentered(const std::string& text);
		static void TextCentered(const std::string& text, float maxWidth);

		static bool Button(const std::string& text, const glm::vec2& size = { 0, 0 }, bool disabled = false);

		static bool ImageButton(UITexture texture, const glm::vec2& size, bool disabled = false);
		static bool ImageButton(std::shared_ptr<Pine::Texture2D> texture, const glm::vec2& size, bool disabled = false);

		static void BeginDisabled();
		static void EndDisabled();

		static glm::vec2 GetWindowSpaceMousePosition();

		static ImVec4 GetColor(const std::string& name);
	};

}
