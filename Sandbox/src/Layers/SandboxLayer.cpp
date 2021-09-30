#include "SandboxLayer.h"

#include <string>

void SandboxLayer::OnAttach()
{
	PINE_LOG_INFO("Press any key with while holding shift to print the pressed key");

	Pine::EventDispatcher<Pine::WindowResizeEvent>::Listen([](const Pine::WindowResizeEvent& e, void* data) {
		PINE_LOG_INFO("Window resized to {0}, {1}", e.Width, e.Height);

		return false;
	});

	Pine::EventDispatcher<Pine::KeyPressedEvent>::Listen([](const Pine::KeyPressedEvent& e, void* data) {
		if (e.key != Pine::Key::LeftShift && Pine::Input::isKeyPressed(Pine::Key::LeftShift)) {
			PINE_LOG_INFO("Key {0} was pressed", (char)e.key);
		}

		return false;
	});

	std::string vertexSource =
		"#version 450 core\n"
		"layout (location = 0) in vec3 a_Pos;\n"
		"layout (location = 1) in vec4 a_Color;\n"

		"struct VertexOutput {\n"
		"  vec4 Color;\n"
		"};\n"

		"layout (location = 0) out VertexOutput Output;\n"

		"void main() {\n"
		"  gl_Position = vec4(a_Pos, 1.0);\n"
		"  Output.Color = a_Color;\n"
		"}\n";

	std::string fragmentSource =
		"#version 450 core\n"
		"layout (location = 0) out vec4 o_Color;\n"

		"struct VertexOutput {\n"
		"  vec4 Color;\n"
		"};\n"

		"layout (location = 0) in VertexOutput Input;\n"

		"void main() {\n"
		"  o_Color = Input.Color;\n"
		"}\n";

	m_Shader = std::make_shared<Pine::Shader>("Renderer2D_FlatColor", vertexSource, fragmentSource);

	m_VertexArray = std::make_shared<Pine::VertexArray>();

	{
		uint32_t indices[] = {
			0, 1, 3,
			1, 2, 3,
		};
		auto indexBuffer = std::make_shared<Pine::IndexBuffer>(indices, 6);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	{
		float vertices[] = {
			// positions        // colors              
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		Pine::BufferLayout layout = {
			{ Pine::ShaderDataType::Float3, "a_Position" },
			{ Pine::ShaderDataType::Float4, "a_Color" },
		};

		auto vertexBuffer = std::make_shared<Pine::VertexBuffer>(vertices, sizeof(vertices), layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
	}
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(Pine::Timestep ts)
{
	Pine::RenderCommand::ClearColorBufferBit();
	Pine::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.4f });

	Pine::RenderCommand::DrawIndexed(m_VertexArray, m_Shader);
}
