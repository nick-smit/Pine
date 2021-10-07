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
	}

	void SceneHierarchyPanel::OnDetach()
	{
		for (auto unsub : m_EventListeners) {
			unsub();
		}
	}

	void SceneHierarchyPanel::OnRender(Timestep ts)
	{
		ImGui::Begin("Scene hierarchy");

		m_SceneContext->GetContext()->GetEnttRegistry().each([&](auto entityID) {
			Entity entity(entityID, m_SceneContext->GetContext().get());
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		bool selected = entity == m_SelectedEntity;

		if (ImGui::Selectable(entity.GetTag().c_str(), &selected)) {
			EventDispatcher<EntitySelectedEvent>::Dispatch({ entity });
		}
	}

}
