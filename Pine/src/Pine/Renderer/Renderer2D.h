#pragma once

#include <glm\glm.hpp>

namespace Pine {

	class Renderer2D {
	public:
		Renderer2D() = delete;
		~Renderer2D() = delete;

		static void Init();
		static void Terminate();

		static void BeginScene();
		static void EndScene();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4&  color);
	};

}