#pragma once

#include "Panel.h"

#include <Pine.h>
#include <memory>

namespace Pine {

	class ViewportPanel : public Panel {
	public:
		ViewportPanel() = default;
		~ViewportPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;


	public:
		static std::string GetName() { return "ViewportPanel"; };

	private:
		std::unique_ptr<Texture2D> m_Texture = nullptr;
	};

}
