#pragma once

#include "Panel.h"

namespace Pine {

	class ImGuiDemoPanel : public Panel
	{
	public:
		ImGuiDemoPanel() = default;
		~ImGuiDemoPanel() = default;

		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "ImGuiDemoPanel"; }
	};

}