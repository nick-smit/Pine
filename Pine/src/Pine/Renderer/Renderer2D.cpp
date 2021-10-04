#include "pinepch.h"
#include "Renderer2D.h"

#include "Buffers.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glm\gtc\matrix_transform.hpp>

#define USE_BATCH_RENDERER 1

namespace Pine {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData {
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		const uint32_t MaxTextureSlots = 32;
		std::array<std::shared_ptr<Texture2D>, 32> Textures;
		uint32_t TextureIndex = 1; // 0 = white texture

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPositions[4];

		std::shared_ptr<Shader> TextureShader;
	};

	static Renderer2DData* s_Data;
	
	void Renderer2D::Init()
	{
		PINE_PROFILE_FUNCTION();

		s_Data = new Renderer2DData();

		s_Data->QuadVertexArray = std::make_shared<VertexArray>();

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float2, "a_TexCoords" },
				{ ShaderDataType::Float1, "a_TexIndex" },
				{ ShaderDataType::Float1, "a_TilingFactor" }
			};
			s_Data->QuadVertexBuffer = std::make_shared<VertexBuffer>(s_Data->MaxVertices * sizeof(QuadVertex), layout);
			s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);
		}

		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

		{
			uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];

			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6) {
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;

				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;

				offset += 4;
			}

			auto quadIndexBuffer = std::make_shared<IndexBuffer>(quadIndices, s_Data->MaxIndices);
			s_Data->QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

			delete[] quadIndices;
		}

		{
			Texture2D::Specification spec;
			uint32_t data = 0xFFFFFFFF;
			s_Data->Textures[0] = std::make_shared<Texture2D>(spec, 1, 1, 4, &data, sizeof(uint32_t));
		}

		s_Data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data->TextureShader = Shader::FromFile("Renderer2D_QuadShader", "./assets/shaders/Renderer2D_QuadShader.glsl");
	}

	void Renderer2D::Terminate()
	{
		PINE_PROFILE_FUNCTION();

		delete[] s_Data->QuadVertexBufferBase;
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		PINE_PROFILE_FUNCTION();

		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::DrawQuad(const QuadSpecification& spec)
	{
		PINE_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		float textureIndex = 0; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices || s_Data->TextureIndex >= s_Data->MaxTextureSlots) {
			Flush();
			StartBatch();
		}

		if (spec.Texture) {
			for (uint32_t i = 1; i < s_Data->TextureIndex; i++)
			{
				if (s_Data->Textures[i].get() == spec.Texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			// If we didn't find a texture we should add it to the array.
			if (textureIndex == 0) {
				s_Data->Textures[s_Data->TextureIndex] = spec.Texture;
				textureIndex = (float)s_Data->TextureIndex;
				s_Data->TextureIndex++;
			}
		}

		glm::mat4 transform;
		if (!spec.UseTransform) {
			transform = glm::translate(glm::mat4(1.0f), spec.Position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(spec.ZRotation), { 0.0f, 0.0f, 1.0f }) *
				glm::scale(glm::mat4(1.0f), spec.Scale);
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = (spec.UseTransform ? spec.Transform : transform) * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = spec.Color;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->TilingFactor = spec.TilingFactor;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::StartBatch()
	{
		PINE_PROFILE_FUNCTION();

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

		s_Data->TextureIndex = 1;
	}

	void Renderer2D::Flush()
	{
		PINE_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount) {
			// Draw quads
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase);
			s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data->TextureIndex; i++) {
				s_Data->Textures[i]->Bind(i);
			}

			RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->TextureShader, s_Data->QuadIndexCount);
		}
	}

}
