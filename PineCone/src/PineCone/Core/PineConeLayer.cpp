#include "PineConeLayer.h"

#include "PineCone\Panels\ImGuiDemoPanel.h"

#include <Pine.h>

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "backends\imgui_impl_opengl3.h"
#include "backends\imgui_impl_glfw.h"

#include <GLFW\glfw3.h>

Pine::PineConeLayer::PineConeLayer()
	: Layer("PineCone_PineConeLayer")
{
	m_panelManager.AddPanel(ImGuiDemoPanel::GetName(), std::make_shared<ImGuiDemoPanel>(), true);
}

Pine::PineConeLayer::~PineConeLayer()
{
	for (auto fn : m_UnsubscribeFunctions) {
		fn();
	}
}

void Pine::PineConeLayer::OnAttach()
{
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

void Pine::PineConeLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Pine::PineConeLayer::OnUpdate(Timestep ts)
{
	RenderCommand::Clear();
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	{
		m_panelManager.OnRender(ts);
	}

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
