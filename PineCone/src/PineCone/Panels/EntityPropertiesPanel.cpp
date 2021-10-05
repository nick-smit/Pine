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

}
