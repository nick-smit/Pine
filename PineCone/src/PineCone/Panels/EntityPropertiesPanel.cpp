#include "pcpch.h"
#include "EntityPropertiesPanel.h"
#include "PineCone\Core\Event.h"

#include "PineCone\ImGui\UI.h"

#include <imgui.h>
#include <glm\glm.hpp>

namespace Pine {

	void EntityPropertiesPanel::OnAttach()
	{
		m_EventListeners.push_back(EventDispatcher<EntitySelectedEvent>::Listen([&](const EntitySelectedEvent& e) {
			m_SelectedEntity = e.Entity;

			return false;
		}));

		m_EventListeners.push_back(EventDispatcher<SceneOpenedEvent>::Listen([&](const SceneOpenedEvent& e) {
			m_SelectedEntity = Entity();

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
		PINE_PROFILE_FUNCTION();

		ImGui::Begin("Properties");

		if (m_SelectedEntity)
			// DrawTagComponent might destroy the selected entity
			DrawTagComponent();

		if (m_SelectedEntity) {
			DrawComponent<TransformComponent>("Transform component", [](TransformComponent& transformComponent) {
				float speed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl) ? 0.01f : 0.1f;

				UI::Vec3Control("Translation", transformComponent.Translation);
				
				glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
				UI::Vec3Control("Rotation", rotation);
				transformComponent.Rotation = glm::radians(rotation);
				
				UI::Vec3Control("Scale", transformComponent.Scale, 1.0f);
			});
			
			DrawComponent<SpriteRendererComponent>("Sprite renderer component", [](SpriteRendererComponent& spriteRenderer) {
				UI::ColorEdit4("Color", spriteRenderer.Color);
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

			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 8.0f);
			if (UI::Button(UITexture::VerticalDots, { 8.0f, 16.0f }, ImGui::IsPopupOpen("Add component popup") ? UI::Status::Active : UI::Status::None)) {
				ImGui::OpenPopup("Add component popup");
			}

			if (ImGui::BeginPopup("Add component popup")) {
				if (!m_SelectedEntity.HasComponent<TransformComponent>()) {
					if (ImGui::MenuItem("Transform")) {
						m_SelectedEntity.AddComponent<TransformComponent>();
					}
				}

				if (!m_SelectedEntity.HasComponent<SpriteRendererComponent>()) {
					if (ImGui::MenuItem("Sprite renderer")) {
						m_SelectedEntity.AddComponent<SpriteRendererComponent>();
					}
				}

				if (m_SelectedEntity.HasAllComponents<TransformComponent, SpriteRendererComponent>()) {
					ImGui::MenuItem("No components to add", NULL, false, false);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Remove entity")) {
					m_SceneContext->GetContext()->DestroyEntity(m_SelectedEntity);
					EventDispatcher<EntitySelectedEvent>::Dispatch({ Entity() });
				}

				ImGui::EndPopup();
			}

		}
	}
}
