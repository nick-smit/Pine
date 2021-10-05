#pragma once

#include "Panel.h"

#include <Pine.h>
#include <functional>
#include <memory>
#include <vector>

namespace Pine {

	class EntityPropertiesPanel : public Panel
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "EntityPropertiesPanel"; };
	private:
		void DrawTagComponent();

	private:
		std::vector<std::function<void()>> m_EventListeners;

		Entity m_SelectedEntity;
	};

}
