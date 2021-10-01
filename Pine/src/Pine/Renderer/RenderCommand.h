#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

namespace Pine {

	class RenderCommand {
	public:
		static void SetViewportSize(uint32_t width, uint32_t height);

		static void Draw(uint32_t indices);
		static void DrawIndexed(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Shader> shader);
		static void DrawIndexed(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Shader> shader, uint32_t count);

		static void Clear();

		static void SetClearColor(glm::vec3 color);
		static void SetClearColor(glm::vec4 color);

		static void SetPolygonMode(Renderer::Face face, Renderer::PolygonMode mode);
	};

}