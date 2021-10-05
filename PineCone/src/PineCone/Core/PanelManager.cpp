#include "PanelManager.h"

namespace Pine {

	PanelManager* PanelManager::s_Instance = nullptr;

	PanelManager::PanelManager()
	{
		PINE_ASSERT(!s_Instance, "Multiple instances of PanelManager are not allowed!");
		s_Instance = this;
	}

	PanelManager::~PanelManager()
	{
		PINE_PROFILE_FUNCTION();

		for (auto panel : m_ActivePanels) {
			panel->OnDetach();
		}

		for (auto panel : m_Panels) {
			delete panel.second;
		}
	}

	bool PanelManager::IsPanelActive(const std::string& name)
	{
		return m_PanelStates[name];
	}

	void PanelManager::AddPanel(const std::string& name, Panel* panel, bool activate)
	{
		PINE_PROFILE_FUNCTION();

		m_Panels[name] = panel;
		m_PanelStates[name] = activate;
		
		if (activate) {
			panel->OnAttach();
			m_ActivePanels.push_back(panel);
		}
	}

	void PanelManager::ActivatePanel(const std::string& name)
	{
		PINE_PROFILE_FUNCTION();

		PINE_ASSERT(m_Panels.find(name) != m_Panels.end(), "Panel '{0}' does not exist.", name);
		PINE_ASSERT(!m_PanelStates[name], "Panel '{0}' is already active.", name);

		m_PanelStates[name] = true;
		m_PanelsToActivate.insert(m_Panels[name]);
	}

	void PanelManager::DeactivatePanel(const std::string& name)
	{
		PINE_PROFILE_FUNCTION();

		PINE_ASSERT(m_Panels.find(name) != m_Panels.end(), "Panel '{0}' does not exist.", name);
		PINE_ASSERT(m_PanelStates[name], "Panel '{0}' is not active.", name);

		m_PanelStates[name] = false;
		m_PanelsToDeactivate.insert(m_Panels[name]);
	}

	void PanelManager::OnRender(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		for (auto panel : m_ActivePanels) {
			panel->OnRender(ts);
		}

		if (m_PanelsToActivate.size()) {
			PINE_PROFILE_SCOPE("Activating panels");

			for (Panel* panel : m_PanelsToActivate) {
				panel->OnAttach();
				m_ActivePanels.push_back(panel);
			}
			m_PanelsToActivate.clear();
		}

		if (m_PanelsToDeactivate.size()) {
			PINE_PROFILE_SCOPE("Deactivating panels");

			for (auto panel : m_PanelsToDeactivate) {
				auto panelPtr = std::find(m_ActivePanels.begin(), m_ActivePanels.end(), panel);
				(*panelPtr)->OnDetach();
				m_ActivePanels.erase(panelPtr);
			}
			m_PanelsToDeactivate.clear();
		}
	}

}