#include "pcpch.h"
#include "PineConeLayer.h"

#include "Event.h"

#include "PineCone\Panels\EditorPropertiesPanel.h"
#include "PineCone\Panels\EntityPropertiesPanel.h"
#include "PineCone\Panels\MenuBarPanel.h"
#include "PineCone\Panels\ImGuiDemoPanel.h"
#include "PineCone\Panels\SceneHierarchyPanel.h"
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
		m_SceneContext = std::make_shared<SceneContext>();
		m_CameraController = std::make_shared<EditorCameraController>();
	}

	void PineConeLayer::OnAttach()
	{
		PINE_PROFILE_FUNCTION();

		m_EditorScene = std::make_shared<Scene>();

		m_SceneContext->SetContext(m_EditorScene);

		// setup renderer related stuff
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(fbSpec);

		m_CameraController->ResizeViewport({ (float)fbSpec.Width, (float)fbSpec.Height });
		m_CameraController->Initialize();

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
			m_UnsubscribeFunctions.push_back(EventDispatcher<ViewportFocusedEvent>::Listen([&](const ViewportFocusedEvent& e) {
				m_ViewportInFocus = e.Status;

				ImGuiIO& io = ImGui::GetIO();
				if (m_ViewportInFocus) {
					// Disable keyboard navidation when viewport is in focus
					io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
				}
				else {
					io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
				}

				return false;
			}));

			// Mouse events
			m_UnsubscribeFunctions.push_back(EventDispatcher<MouseButtonPressedEvent>::Listen([&](const MouseButtonPressedEvent& e) {
				return !m_ViewportInFocus && ImGui::GetIO().WantCaptureMouse;
			}));
			m_UnsubscribeFunctions.push_back(EventDispatcher<MouseButtonReleasedEvent>::Listen([&](const MouseButtonReleasedEvent& e) {
				return !m_ViewportInFocus && ImGui::GetIO().WantCaptureMouse;
			}));
			m_UnsubscribeFunctions.push_back(EventDispatcher<MouseMovedEvent>::Listen([&](const MouseMovedEvent& e) {
				return !m_ViewportInFocus && ImGui::GetIO().WantCaptureMouse;
			}));
			m_UnsubscribeFunctions.push_back(EventDispatcher<MouseScrolledEvent>::Listen([&](const MouseScrolledEvent& e) {
				return !m_ViewportInFocus && ImGui::GetIO().WantCaptureMouse;
			}));

			// Keyboard events
			m_UnsubscribeFunctions.push_back(EventDispatcher<KeyPressedEvent>::Listen([&](const KeyPressedEvent& e) {
				return !m_ViewportInFocus && ImGui::GetIO().WantCaptureKeyboard;
			}));

			m_UnsubscribeFunctions.push_back(EventDispatcher<KeyReleasedEvent>::Listen([&](const KeyReleasedEvent& e) {
				return !m_ViewportInFocus && ImGui::GetIO().WantCaptureKeyboard;
			}));

			m_UnsubscribeFunctions.push_back(EventDispatcher<KeyRepeatedEvent>::Listen([&](const KeyRepeatedEvent& e) {
				return !m_ViewportInFocus && ImGui::GetIO().WantCaptureKeyboard;
			}));

			m_UnsubscribeFunctions.push_back(EventDispatcher<SceneOpenedEvent>::Listen([&](const SceneOpenedEvent& e) {
				m_EditorScene = SceneSerializer::Deserialize(e.Filepath);
				m_SceneContext->SetContext(m_EditorScene);

				return false;
			}));

			m_UnsubscribeFunctions.push_back(EventDispatcher<SceneSavedEvent>::Listen([&](const SceneSavedEvent& e) {
				SceneSerializer::Serialize(m_EditorScene, e.Filepath);

				return false;
			}));
		}

		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::OnAttach - Add panels");

			m_PanelManager.AddPanel(MenuBarPanel::GetName(), new MenuBarPanel(), true);
			m_PanelManager.AddPanel(SceneHierarchyPanel::GetName(), new SceneHierarchyPanel(m_SceneContext, m_CameraController), true);
			m_PanelManager.AddPanel(EntityPropertiesPanel::GetName(), new EntityPropertiesPanel(m_SceneContext), true);
			m_PanelManager.AddPanel(ViewportPanel::GetName(), new ViewportPanel(m_Framebuffer), true);
			m_PanelManager.AddPanel(EditorPropertiesPanel::GetName(), new EditorPropertiesPanel(m_CameraController), true);

			#if PINE_DEBUG
			m_PanelManager.AddPanel(ImGuiDemoPanel::GetName(), new ImGuiDemoPanel());
			#endif
		}
	}

	void PineConeLayer::OnDetach()
	{
		PINE_PROFILE_FUNCTION();

		for (auto fn : m_UnsubscribeFunctions) {
			fn();
		}

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

				m_CameraController->ResizeViewport(viewportSize);
			}
		}

		m_CameraController->Update(ts);

		{
			PINE_PROFILE_SCOPE("Pine::PineConeLayer::UpdateScene - Draw Framebuffer");

			m_Framebuffer->Bind();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			SceneRenderer::RenderScene(m_EditorScene, m_CameraController->GetCamera());

			m_Framebuffer->Unbind();
		}
	}
	
}
