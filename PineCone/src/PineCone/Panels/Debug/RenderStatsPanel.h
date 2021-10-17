#pragma once

#include "PineCone\Panels\Panel.h"

namespace Pine {

	class RenderStatsPanel : public Panel
	{
	public:
		RenderStatsPanel() = default;
		virtual ~RenderStatsPanel() = default;

		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "RenderStatsPanel"; };
	};

}
