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
		for (auto panels : m_ActivePanels) {
			panels->OnDetach();
		}
	}

	bool PanelManager::IsPanelActive(const std::string& name)
	{
		return m_PanelStates[name];
	}

	void PanelManager::AddPanel(const std::string& name, std::shared_ptr<Panel> panel, bool activate)
	{
		m_Panels[name] = panel;
		m_PanelStates[name] = activate;
		
		if (activate) {
			panel->OnAttach();
			m_ActivePanels.push_back(panel);
		}
	}

	void PanelManager::ActivatePanel(const std::string& name)
	{
		PINE_ASSERT(m_Panels.find(name) != m_Panels.end(), "Panel '{0}' does not exist.", name);
		PINE_ASSERT(!m_PanelStates[name], "Panel '{0}' is already active.", name);

		auto panel = m_Panels[name];

		m_PanelStates[name] = true;
		panel->OnAttach();
		m_ActivePanels.push_back(panel);
	}

	void PanelManager::DeactivatePanel(const std::string& name)
	{
		PINE_ASSERT(m_Panels.find(name) != m_Panels.end(), "Panel '{0}' does not exist.", name);
		PINE_ASSERT(m_PanelStates[name], "Panel '{0}' is not active.", name);

		auto panelPtr = std::find(m_ActivePanels.begin(), m_ActivePanels.end(), m_Panels[name]);

		m_PanelStates[name] = false;
		(*panelPtr)->OnDetach();
		m_ActivePanels.erase(panelPtr);
	}

	void PanelManager::OnRender(Timestep ts) const
	{
		for (auto panel : m_ActivePanels) {
			panel->OnRender(ts);
		}
	}

}