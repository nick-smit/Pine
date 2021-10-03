#include "PineConeLayer.h"

#include "PineCone\Panels\ImGuiDemoPanel.h"

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "backends\imgui_impl_opengl3.h"
#include "backends\imgui_impl_glfw.h"


Pine::PineConeLayer::PineConeLayer()
	: Layer("PineCone_PineConeLayer")
{
	m_Panels.push_back(std::make_shared<ImGuiDemoPanel>());
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
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(Application::Get()->GetWindow()->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
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

	{
		for (auto panel : m_Panels) {
			panel->OnRender(ts);
		}
	}

	ImGui::Render();
	//int display_w, display_h;
	//glfwGetFramebufferSize(Application::Get()->GetWindow()->GetNativeWindow(), &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
