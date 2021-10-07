#pragma once

#include "Panel.h"
#include "PineCone\Core\SceneContext.h"

#include <Pine.h>

#include <memory>

namespace Pine {

	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel(std::shared_ptr<SceneContext> context)
			: m_SceneContext(context) {};
		virtual ~SceneHierarchyPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "SceneHierarchyPanel"; };

	private:
		void DrawEntityNode(Entity entity);

	private:
		std::vector<std::function<void()>> m_EventListeners;

		std::shared_ptr<SceneContext> m_SceneContext;
		Entity m_SelectedEntity;
	};

}
