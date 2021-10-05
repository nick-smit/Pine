#pragma once

#include "Panel.h"

#include <Pine.h>

#include <memory>

namespace Pine {

	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel(std::shared_ptr<Scene> context);
		virtual ~SceneHierarchyPanel() = default;

		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "SceneHierarchyPanel"; };

	private:
		void DrawEntityNode(Entity entity);

	private:
		std::shared_ptr<Scene> m_Context;

		Entity m_SelectedEntity;
	};

}
