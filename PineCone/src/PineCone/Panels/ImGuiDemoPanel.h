#pragma once

#include "Panel.h"

namespace Pine {

	class ImGuiDemoPanel : public Panel
	{
	public:
		ImGuiDemoPanel();
		~ImGuiDemoPanel();

		virtual void OnRender(Timestep ts) override;

	public:
		static const std::string& GetName() { return "ImGuiDemoPanel"; }
	};

}