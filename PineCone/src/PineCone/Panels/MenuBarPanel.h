#pragma once

#include "Panel.h"

namespace Pine {

	class MenuBarPanel : public Panel
	{
	public:
		MenuBarPanel() = default;
		~MenuBarPanel() = default;

		virtual void OnAttach() override;
		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "MenuBarPanel"; };
	};

}