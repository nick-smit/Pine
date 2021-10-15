#include "pcpch.h"
#include "SceneHierarchyPanel.h"
#include "PineCone\Core\Event.h"

#include <Pine.h>
#include <entt\entt.hpp>
#include <imgui.h>

namespace Pine {

	void SceneHierarchyPanel::OnAttach()
	{
		m_EventListeners.push_back(EventDispatcher<EntitySelectedEvent>::Listen([&](const EntitySelectedEvent& e) {
			m_SelectedEntity = e.Entity;

			return false;
		}));

		m_EventListeners.push_back(EventDispatcher<SceneOpenedEvent>::Listen([&](const SceneOpenedEvent& e) {
			m_SelectedEntity = Entity();

			return false;
		}));

		m_EventListeners.push_back(EventDispatcher<KeyPressedEvent>::Listen([&](const KeyPressedEvent& e) {
			if (e.Key == Key::F) {
				if (m_SelectedEntity && m_SelectedEntity.HasComponent<TransformComponent>()) {
					auto& transformComponent = m_SelectedEntity.GetComponent<TransformComponent>();
					m_EditorCamera->Focus(transformComponent.Translation);

					return true;
				}
			}

			return false;
		}));
	}

	void SceneHierarchyPanel::OnDetach()
	{
		for (auto unsub : m_EventListeners) {
			unsub();
		}
	}

	void SceneHierarchyPanel::OnRender(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		ImGui::Begin("Scene hierarchy");

		if (ImGui::BeginPopupContextWindow(0, ImGuiMouseButton_Right, false)) {
			if (ImGui::MenuItem("Create empty entity")) {
				Entity entity = m_SceneContext->GetContext()->CreateEntity("Empty Entity");
				EventDispatcher<EntitySelectedEvent>::Dispatch({ entity });

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		m_SceneContext->GetContext()->GetEnttRegistry().each([&](auto entityID) {
			Entity entity(entityID, m_SceneContext->GetContext().get());
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		bool selected = entity == m_SelectedEntity;

		ImGui::PushID(std::string("SceneHierarchyPanel::DrawEntityNode##").append(std::to_string(entity)).c_str());
		if (ImGui::Selectable(entity.GetTag().c_str(), &selected)) {
			EventDispatcher<EntitySelectedEvent>::Dispatch({ entity });
		}
		ImGui::PopID();

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Remove entity")) {
				m_SceneContext->GetContext()->DestroyEntity(entity);
				if (selected)
					EventDispatcher<EntitySelectedEvent>::Dispatch({ Entity() });

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

}
