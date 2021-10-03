#pragma once

#include "PineCone\Panels\Panel.h"

#include <Pine.h>

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Pine {

	class PanelManager {
	public:
		PanelManager();
		~PanelManager();

		std::shared_ptr<Panel> GetPanel(const std::string& name);
		void AddPanel(const std::string& name, std::shared_ptr<Panel> panel, bool activate = false);

		void ActivatePanel(const std::string& name);
		void DeactivatePanel(const std::string& name);

		void OnRender(Timestep ts) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Panel>> m_Panels;
		std::vector<std::shared_ptr<Panel>> m_ActivePanels;
	};

}
