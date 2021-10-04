#include "PineConeLayer.h"

#include "PineCone\Panels\MenuBarPanel.h"
#include "PineCone\Panels\ImGuiDemoPanel.h"
#include "PineCone\Panels\ViewportPanel.h"

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "backends\imgui_impl_opengl3.h"
#include "backends\imgui_impl_glfw.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

namespace Pine {

	PineConeLayer::PineConeLayer()
		: Layer("PineCone_PineConeLayer")
	{
	}

	PineConeLayer::~PineConeLayer()
	{
		PINE_PROFILE_FUNCTION();

		for (auto fn : m_UnsubscribeFunctions) {
			fn();
		}
	}

	void PineConeLayer::OnAttach()
	{
		PINE_PROFILE_FUNCTION();

		Scene scene;
		scene.CreateEntity("MyEntity");

		// setup renderer related stuff
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(fbSpec);

		float aspectRatio = (float)fbSpec.Width / (float)fbSpec.Height;
		m_Camera.SetProjection(-aspectRatio, aspectRatio, -1.0f, 1.0f);

		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::OnAttach - Add panels");
			
			m_PanelManager.AddPanel(MenuBarPanel::GetName(), std::make_shared<MenuBarPanel>(), true);
			m_PanelManager.AddPanel(ViewportPanel::GetName(), std::make_shared<ViewportPanel>(m_Framebuffer), true);

			#if PINE_DEBUG
			m_PanelManager.AddPanel(ImGuiDemoPanel::GetName(), std::make_shared<ImGuiDemoPanel>(), true);
			#endif
		}

		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::OnAttach - Setup ImGui");

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(Application::Get()->GetWindow()->GetNativeWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 450");

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
		}

		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::OnAttach - Register event handlers");

			// Setup event handlers
			// Mouse events
			m_UnsubscribeFunctions.push_back(EventDispatcher<MouseButtonPressedEvent>::Listen([](const MouseButtonPressedEvent& e) {
				ImGuiIO& io = ImGui::GetIO();
				return io.WantCaptureMouse;
				}));
			m_UnsubscribeFunctions.push_back(EventDispatcher<MouseButtonReleasedEvent>::Listen([](const MouseButtonReleasedEvent& e) {
				ImGuiIO& io = ImGui::GetIO();
				return io.WantCaptureMouse;
				}));
			m_UnsubscribeFunctions.push_back(EventDispatcher<MouseMovedEvent>::Listen([](const MouseMovedEvent& e) {
				ImGuiIO& io = ImGui::GetIO();
				return io.WantCaptureMouse;
				}));
			m_UnsubscribeFunctions.push_back(EventDispatcher<MouseScrolledEvent>::Listen([](const MouseScrolledEvent& e) {
				ImGuiIO& io = ImGui::GetIO();
				return io.WantCaptureMouse;
				}));

			// Keyboard events
			m_UnsubscribeFunctions.push_back(EventDispatcher<KeyPressedEvent>::Listen([](const KeyPressedEvent& e) {
				return ImGui::GetIO().WantCaptureKeyboard;
				}));

			m_UnsubscribeFunctions.push_back(EventDispatcher<KeyReleasedEvent>::Listen([](const KeyReleasedEvent& e) {
				return ImGui::GetIO().WantCaptureKeyboard;
				}));

			m_UnsubscribeFunctions.push_back(EventDispatcher<KeyRepeatedEvent>::Listen([](const KeyRepeatedEvent& e) {
				return ImGui::GetIO().WantCaptureKeyboard;
				}));
		}
	}

	void PineConeLayer::OnDetach()
	{
		PINE_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void PineConeLayer::OnUpdate(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		UpdateScene(ts);

		RenderCommand::Clear();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		BeginDockspace();

		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::OnUpdate() - Render Panels");

			m_PanelManager.OnRender(ts);
		}

		EndDockspace();

		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::OnUpdate() - ImGui Draw");

			ImGuiIO& io = ImGui::GetIO();
			Application* app = Application::Get();
			io.DisplaySize = ImVec2((float)app->GetWindow()->GetSpecification().Width, (float)app->GetWindow()->GetSpecification().Height);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				GLFWwindow* originalContext = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(originalContext);
			}
		}
	}

	void PineConeLayer::BeginDockspace() const
	{
		PINE_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar(3);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		style.WindowMinSize.x = minWinSizeX;
	}

	void PineConeLayer::EndDockspace() const
	{
		PINE_PROFILE_FUNCTION();

		ImGui::End();
	}

	void PineConeLayer::UpdateScene(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		// Resize framebuffer if necessary
		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::UpdateScene - Resize Framebuffer");

			FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			auto viewportPanel = m_PanelManager.GetPanel<ViewportPanel>(ViewportPanel::GetName());
			const glm::vec2& viewportSize = viewportPanel->GetSize();

			if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.Width != viewportSize.x || spec.Height != viewportSize.y))
			{
				m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);

				float aspectRatio = viewportSize.x / viewportSize.y;
				m_Camera.SetProjection(-aspectRatio, aspectRatio, -1.0f, 1.0f);
			}
		}

		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::UpdateScene - Draw Framebuffer");

			m_Framebuffer->Bind();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			Renderer2D::BeginScene(m_Camera);

			Renderer2D::QuadSpecification quadSpec;
			quadSpec.Color = { 0.3f, 0.7f, 0.1f, 1.0f };
			quadSpec.Position = { 0.0f, 0.0f, 0.0f };
			Renderer2D::DrawQuad(quadSpec);

			Renderer2D::EndScene();

			m_Framebuffer->Unbind();
		}
	}
	
}
