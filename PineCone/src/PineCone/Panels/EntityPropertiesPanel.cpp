#include "pcpch.h"
#include "EntityPropertiesPanel.h"
#include "PineCone\Core\Event.h"

#include <imgui.h>
#include <glm\glm.hpp>

namespace Pine {

	void EntityPropertiesPanel::OnAttach()
	{
		m_EventListeners.push_back(EventDispatcher<EntitySelectedEvent>::Listen([&](const EntitySelectedEvent& e) {
			m_SelectedEntity = e.Entity;

			return false;
		}));
	}

	void EntityPropertiesPanel::OnDetach()
	{
		for (auto unsub : m_EventListeners) {
			unsub();
		}
	}

	void EntityPropertiesPanel::OnRender(Timestep ts)
	{
		ImGui::Begin("Properties");

		if (m_SelectedEntity) {
			DrawTagComponent();

			DrawComponent<TransformComponent>("Transform", [](TransformComponent& transformComponent) {
				ImGui::DragFloat3("Translation", &transformComponent.Translation.x, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f);

				glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
				ImGui::DragFloat3("Rotation", &rotation.x, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f);
				transformComponent.Rotation = glm::radians(rotation);

				ImGui::DragFloat3("Scale", &transformComponent.Scale.x, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f);
			});
			
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", [](SpriteRendererComponent& spriteRenderer) {
				ImGui::ColorEdit4("Color", &spriteRenderer.Color.r);
			});
		}

		ImGui::End();
	}

	void EntityPropertiesPanel::DrawTagComponent()
	{
		PINE_ASSERT(m_SelectedEntity.HasComponent<TagComponent>(), "Entity without tagcomponent!");

		if (m_SelectedEntity.HasComponent<TagComponent>())
		{
			auto& tag = m_SelectedEntity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

			PC_IMGUI_SAME_LINE_SPACE_BETWEEN_TEXT("Add");
			if (ImGui::Button("Add")) {
				ImGui::OpenPopup("Add component popup");
			}
			if (ImGui::BeginPopup("Add component popup")) {
				if (!m_SelectedEntity.HasComponent<TransformComponent>()) {
					if (ImGui::MenuItem("Transform")) {
						m_SelectedEntity.AddComponent<TransformComponent>();
					}
				}

				if (!m_SelectedEntity.HasComponent<SpriteRendererComponent>()) {
					if (ImGui::MenuItem("Sprite Renderer")) {
						m_SelectedEntity.AddComponent<SpriteRendererComponent>();
					}
				}

				if (m_SelectedEntity.HasAllComponents<TransformComponent, SpriteRendererComponent>()) {
					ImGui::MenuItem("No components to add", NULL, false, false);
				}

				ImGui::EndPopup();
			}

		}
	}
}
