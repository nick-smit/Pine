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
		PanelManager(const PanelManager& other) = delete;
		virtual ~PanelManager();

		template<typename T>
		T* GetPanel(const std::string& name) {
			PINE_ASSERT(m_Panels.find(name) != m_Panels.end(), "Panel '{0}' does not exist.", name);

			return (T*) m_Panels[name];
		};
		void AddPanel(const std::string& name, Panel* panel, bool activate = false);

		void ActivatePanel(const std::string& name);
		void DeactivatePanel(const std::string& name);

		bool IsPanelActive(const std::string& name);

		void OnRender(Timestep ts) const;

	public:
		static PanelManager* Get() { return s_Instance; }

	private:
		std::unordered_map<std::string, Panel*> m_Panels;
		std::unordered_map<std::string, bool> m_PanelStates;
		std::vector<Panel*> m_ActivePanels;

	private:
		static PanelManager* s_Instance;
	};

}
