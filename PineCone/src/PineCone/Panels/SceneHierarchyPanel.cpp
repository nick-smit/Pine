#include "SceneHierarchyPanel.h"

#include <Pine.h>
#include <entt\entt.hpp>
#include <imgui.h>

namespace Pine {

	SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<Scene> context)
		: m_Context(context), m_SelectedEntity( entt::null, nullptr )
	{
	}

	void SceneHierarchyPanel::OnRender(Timestep ts)
	{
		ImGui::Begin("Scene hierarchy");

		m_Context->GetEnttRegistry().each([&](auto entityID) {
			Entity entity(entityID, m_Context.get());
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		bool selected = entity == m_SelectedEntity;

		if (ImGui::Selectable(entity.GetTag().c_str(), &selected)) {
			m_SelectedEntity = entity;
		}
	}

}
