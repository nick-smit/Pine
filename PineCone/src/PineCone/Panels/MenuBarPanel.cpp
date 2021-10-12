#include "pcpch.h"
#include "MenuBarPanel.h"

#include "ImGuiDemoPanel.h"

#include "PineCone\Core\PanelManager.h"
#include "PineCone\Core\Event.h"
#include "PineCone\Platform\FileDialog.h"

#include <Pine.h>

#include <imgui.h>

namespace Pine {

	static std::vector<FileDialog::Filter> s_SceneFileFilter = { { L"Pine Scene", L"*.pinescene" }, {L"Bitmap", L"*.bmp"}};

	void MenuBarPanel::OnAttach()
	{
		EventDispatcher<KeyPressedEvent>::Listen([&](const KeyPressedEvent& e) {
			bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
			bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

			if (e.Key == Key::O && control) {
				OpenScene();
			}
			else if (e.Key == Key::S && control) {
				if (shift) {
					SaveSceneAs();
				}
				else {
					// Todo
				}
			}

			return false;
		});
	}

	void MenuBarPanel::OnRender(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		PanelManager* panelManager = PanelManager::Get();
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New")) {
					// TODO
				}

				if (ImGui::MenuItem("Open scene...", "Ctrl+O")) {
					OpenScene();
				}

				if (ImGui::MenuItem("Save scene as...", "Ctrl+Shift+S")) {
					SaveSceneAs();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4")) {
					Application::Get()->Close();
				}
		
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {

				#if PINE_DEBUG
				bool imguiDemoPanelActive = panelManager->IsPanelActive(ImGuiDemoPanel::GetName());
				if (ImGui::MenuItem("ImGui Demo", nullptr, &imguiDemoPanelActive)) {
					if (imguiDemoPanelActive)
						panelManager->ActivatePanel(ImGuiDemoPanel::GetName());
					else
						panelManager->DeactivatePanel(ImGuiDemoPanel::GetName());
				}
				#endif

				ImGui::EndMenu();
			}
		
			ImGui::EndMenuBar();
		}
	}

	void MenuBarPanel::OpenScene() const
	{
		auto filepath = FileDialog::OpenFile(s_SceneFileFilter);

		if (!filepath.empty()) {
			EventDispatcher<SceneOpenedEvent>::Dispatch({ filepath });
		}
	}

	void MenuBarPanel::SaveSceneAs() const
	{
		auto filepath = FileDialog::SaveFile(s_SceneFileFilter);

		if (!filepath.empty()) {
			EventDispatcher<SceneSavedEvent>::Dispatch({ filepath });
		}
	}

} 