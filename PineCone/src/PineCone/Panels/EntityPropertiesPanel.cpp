#include "EntityPropertiesPanel.h"
#include "PineCone\Core\Event.h"

#include <imgui.h>

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
			DrawTransformComponent();
			DrawSpriteRendererComponent();
		}

		ImGui::End();
	}

	void EntityPropertiesPanel::DrawTagComponent()
	{
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
		}
	}

	void EntityPropertiesPanel::DrawTransformComponent()
	{
		if (m_SelectedEntity.HasComponent<TransformComponent>())
		{
			auto& transform = m_SelectedEntity.GetComponent<TransformComponent>();
			
			ImGui::DragFloat3("Position", &transform.Position.x, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f);
			ImGui::DragFloat3("Rotation", &transform.Rotation.x, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f);
			ImGui::DragFloat3("Scale", &transform.Scale.x, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f);
		}
	}

	void EntityPropertiesPanel::DrawSpriteRendererComponent()
	{
		if (m_SelectedEntity.HasComponent<SpriteRendererComponent>())
		{
			auto& component = m_SelectedEntity.GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("Color", &component.Color.r);
		}
	}

}
