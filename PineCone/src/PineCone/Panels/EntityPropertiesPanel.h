#pragma once

#include "Panel.h"

#include "PineCone\Core\SceneContext.h"
#include "PineCone\ImGui\ImGuiHelpers.h"

#include <Pine.h>
#include <functional>
#include <memory>
#include <vector>

#include <imgui.h>

namespace Pine {

	class EntityPropertiesPanel : public Panel
	{
	public:
		EntityPropertiesPanel(std::shared_ptr<SceneContext> sceneContext)
			: m_SceneContext(sceneContext) {};

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "EntityPropertiesPanel"; };
	private:
		void DrawTagComponent();

		template<typename Component>
		void DrawComponent(const std::string& name, std::function<void(Component&)> drawFunction) {
			if (!m_SelectedEntity.HasComponent<Component>()) {
				return;
			}

			ImGui::NewLine();

			ImGui::Separator();

			ImGui::Text(name.c_str());

			std::string text = "...";

			PC_IMGUI_SAME_LINE_SPACE_BETWEEN_TEXT(text.c_str());

			ImGui::PushID(std::string("OpenComponentContext##").append(name).c_str());
			if (ImGui::Button(text.c_str())) {
				ImGui::OpenPopup("ComponentContext");
			}

			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentContext")) {
				if (ImGui::MenuItem("Reset to default")) {
					m_SelectedEntity.GetComponent<Component>() = Component();
				}

				if (ImGui::MenuItem("Remove component")) {
					componentRemoved = true;
				}

				ImGui::EndPopup();
			}

			ImGui::PopID();

			if (componentRemoved) {
				m_SelectedEntity.RemoveComponent<Component>();
				return;
			}

			ImGui::NewLine();

			drawFunction(m_SelectedEntity.GetComponent<Component>());

		};

	private:
		std::vector<std::function<void()>> m_EventListeners;

		Entity m_SelectedEntity;
		std::shared_ptr<SceneContext> m_SceneContext;
	};

}
