#include "pinepch.h"
#include "Renderer2D.h"

#include "Buffers.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Pine {

	struct Renderer2DData {
		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<Shader> TextureShader;
	};

	static Renderer2DData* s_Data;
	
	void Renderer2D::Init()
	{
		s_Data = new Renderer2DData();

		s_Data->QuadVertexArray = std::make_shared<VertexArray>();

		{
			float quadVertices[3 * 4] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f,
			};

			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" }
			};

			auto vertexBuffer = std::make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices), layout);
			s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);
		}

		{
			uint32_t quadIndices[6] = {
				0, 1, 3,
				1, 2, 3,
			};
			auto indexBuffer = std::make_shared<IndexBuffer>(quadIndices, 6);
			s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);
		}

		{
			std::string vertexSource =
				"#version 450 core\n"
				"layout (location = 0) in vec3 a_Position;\n"
				//"layout (location = 1) in vec4 a_Color;\n"
				//"layout (location = 2) in vec2 a_TexCoords;\n"

				//"struct VertexOutput {\n"
				//"  vec4 Color;\n"
				//"  vec2 TexCoords;\n"
				//"};\n"

				//"layout (location = 0) out VertexOutput Output;\n"

				"uniform mat4 u_Transform;\n"
				"uniform mat4 u_ViewProjectionMatrix;\n"

				"void main() {\n"
				"  gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0f);\n"
				//"  Output.Color = a_Color;\n"
				//"  Output.TexCoords = a_TexCoords;\n"
				"}\n";

			std::string fragmentSource =
				"#version 450 core\n"
				"layout (location = 0) out vec4 o_Color;\n"

				//"struct VertexOutput {\n"
				//"  vec4 Color;\n"
				//"  vec2 TexCoords;\n"
				//"};\n"

				//"layout (location = 0) in VertexOutput Input;\n"

				//"uniform sampler2D u_BoxTexture;\n"
				"uniform vec4 u_Color;\n"

				"void main() {\n"
				//"  o_Color = texture(u_BoxTexture, Input.TexCoords) * Input.Color;\n"
				"  o_Color = u_Color;\n"
				"}\n";

			s_Data->TextureShader = std::make_shared<Shader>("Renderer2D_FlatColor", vertexSource, fragmentSource);
		}
	}

	void Renderer2D::Terminate()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->TextureShader);
	}

}
