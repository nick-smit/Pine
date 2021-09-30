#include "pinepch.h"
#include "RenderCommand.h"

#include <glad\glad.h>

namespace Pine {



	void RenderCommand::SetViewportSize(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void RenderCommand::Draw(uint32_t indices)
	{
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	}

	void RenderCommand::DrawIndexed(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Shader> shader)
	{
		shader->Bind();
		vertexArray->Bind();

		Draw(vertexArray->GetIndexBuffer()->GetCount());
	}

	void RenderCommand::ClearColorBufferBit()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderCommand::SetClearColor(glm::vec3 color)
	{
		SetClearColor({ color, 1.0f });
	}

	void RenderCommand::SetClearColor(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::SetPolygonMode(Renderer::Face face, Renderer::PolygonMode mode)
	{
		GLenum glFace = GL_NONE;
		switch (face) {
			case Renderer::Face::FrontLeft: glFace = GL_FRONT_LEFT; break;
			case Renderer::Face::FrontRight: glFace = GL_FRONT_RIGHT; break;
			case Renderer::Face::BackLeft: glFace = GL_BACK_LEFT; break;
			case Renderer::Face::BackRight: glFace = GL_BACK_RIGHT; break;
			case Renderer::Face::Front: glFace = GL_FRONT; break;
			case Renderer::Face::Back: glFace = GL_BACK; break;
			case Renderer::Face::Left: glFace = GL_LEFT; break;
			case Renderer::Face::Right: glFace = GL_RIGHT; break;
			case Renderer::Face::FrontAndBack: glFace = GL_FRONT_AND_BACK; break;
			default: PINE_ASSERT(false, "Unknown face type");
		}

		GLenum glMode = GL_NONE;
		switch (mode) {
			case Renderer::PolygonMode::Fill: glMode = GL_FILL; break;
			case Renderer::PolygonMode::Lines: glMode = GL_LINES; break;
			default: PINE_ASSERT(false, "Unknown polygon mode");
		}

		glPolygonMode(glFace, glMode);
	}

}
