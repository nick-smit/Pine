#pragma once

#include <string>

#include <glm\glm.hpp>

namespace Pine {
	
	class UI {
	public:
		static bool DragFloat(const char* label, float* value, float speed = 0.1f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f");

		static void Vec3Control(const char* label, glm::vec3& values, float resetValue = 0.0f);

		static void ColorEdit4(const char* label, glm::vec4& value);

		static void LabeledText(const char* label, const char* text);
	};

}
