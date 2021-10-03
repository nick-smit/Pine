#include "PanelManager.h"

namespace Pine {

	PanelManager::PanelManager()
	{
	}

	PanelManager::~PanelManager()
	{
		for (auto panels : m_ActivePanels) {
			panels->OnDetach();
		}
	}

	std::shared_ptr<Panel> PanelManager::GetPanel(const std::string& name)
	{
		PINE_ASSERT(m_Panels.find(name) != m_Panels.end(), "Panel '{0}' does not exist.", name);

		return m_Panels[name];
	}

	void PanelManager::AddPanel(const std::string& name, std::shared_ptr<Panel> panel, bool activate)
	{
		m_Panels[name] = panel;
		
		if (activate) {
			m_ActivePanels.push_back(panel);
			panel->OnAttach();
		}
	}

	void PanelManager::ActivatePanel(const std::string& name)
	{
		PINE_ASSERT(m_Panels.find(name) != m_Panels.end(), "Panel '{0}' does not exist.", name);

		auto panel = m_Panels[name];
		PINE_ASSERT(std::find(m_ActivePanels.begin(), m_ActivePanels.end(), panel) == m_ActivePanels.end(), "Panel '{0}' is already active.", name);

		m_ActivePanels.push_back(panel);
		panel->OnAttach();
	}

	void PanelManager::DeactivatePanel(const std::string& name)
	{
		PINE_ASSERT(m_Panels.find(name) != m_Panels.end(), "Panel '{0}' does not exist.", name);

		auto panelPtr = std::find(m_ActivePanels.begin(), m_ActivePanels.end(), m_Panels[name]);
		PINE_ASSERT(panelPtr != m_ActivePanels.end(), "Panel '{0}' is not active.", name);

		m_ActivePanels.erase(panelPtr);
	}

	void PanelManager::OnRender(Timestep ts) const
	{
		for (auto panel : m_ActivePanels) {
			panel->OnRender(ts);
		}
	}

}