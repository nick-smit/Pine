#pragma once

#include "Panel.h"

#include "PineCone\Core\SceneContext.h"
#include "PineCone\ImGui\ImGuiHelpers.h"
#include "PineCone\ImGui\UI.h"
#include "PineCone\Command\CommandManager.h"
#include "PineCone\Command\EntityCommand.h"

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

			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 8.0f);

			ImGui::PushID(name.c_str());
			if (UI::Button(UITexture::VerticalDots, { 8.0f, 16.0f }, ImGui::IsPopupOpen("ComponentContext") ? UI::Status::Active : UI::Status::None)) {

				ImGui::OpenPopup("ComponentContext");
			}
			
			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentContext")) {
				if (ImGui::MenuItem("Reset to default")) {
					auto cmd = std::make_shared<ResetComponentCommand<Component>>(m_SceneContext->GetContext(), m_SelectedEntity);
					CommandManager::GetInstance().ExecuteCommand(cmd);
				}

				if (ImGui::MenuItem("Remove component")) {
					componentRemoved = true;
				}

				ImGui::EndPopup();
			}

			ImGui::PopID();

			if (componentRemoved) {
				auto cmd = std::make_shared<RemoveComponentCommand<Component>>(m_SceneContext->GetContext(), m_SelectedEntity);
				CommandManager::GetInstance().ExecuteCommand(cmd);
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
