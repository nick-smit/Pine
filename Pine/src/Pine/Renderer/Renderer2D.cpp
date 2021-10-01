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

		std::shared_ptr<Texture2D> WhiteTexture;
	};

	static Renderer2DData* s_Data;
	
	void Renderer2D::Init()
	{
		s_Data = new Renderer2DData();

		{
			Texture2D::Specification spec;
			uint32_t data = 0xFFFFFFFF;
			s_Data->WhiteTexture = std::make_shared<Texture2D>(spec, 1, 1, 4, &data, sizeof(uint32_t));
		}

		s_Data->QuadVertexArray = std::make_shared<VertexArray>();

		{
			float quadVertices[5 * 4] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			};

			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoords" }
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

		s_Data->TextureShader = Shader::FromFile("Renderer2D_QuadShader", "./assets/shaders/Renderer2D_QuadShader.glsl");
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

	void Renderer2D::DrawQuad(const QuadSpecification& spec)
	{
		if (spec.UseTransform) {
			s_Data->TextureShader->SetMat4("u_Transform", spec.Transform);
		} else {
			glm::mat4 transform;

			transform = glm::translate(glm::mat4(1.0f), spec.Position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(spec.ZRotation), { 0.0f, 0.0f, 1.0f }) *
				glm::scale(glm::mat4(1.0f), spec.Scale);

			s_Data->TextureShader->SetMat4("u_Transform", transform);
		}

		s_Data->TextureShader->SetFloat4("u_Color", spec.Color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", spec.TilingFactor);
		
		if (spec.Texture) {
			spec.Texture->Bind(0);
		}
		else {
			s_Data->WhiteTexture->Bind(0);
		}

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->TextureShader);
	}

}
