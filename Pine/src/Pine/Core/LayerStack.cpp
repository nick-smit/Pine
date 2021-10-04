#include "pinepch.h"
#include "LayerStack.h"

namespace Pine {

	LayerStack::~LayerStack()
	{
		Terminate();
	}

	void LayerStack::Terminate()
	{
		PINE_PROFILE_FUNCTION();

		for (auto layer : m_Layers) {
			layer->OnDetach();
		}

		m_Layers.clear();
	}

	void LayerStack::PushLayer(std::shared_ptr<Layer> layer)
	{
		PINE_PROFILE_FUNCTION();

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		
		layer->OnAttach();
	}

	void LayerStack::PopLayer(std::shared_ptr<Layer> layer)
	{
		PINE_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;

			layer->OnDetach();
		}
	}

	void LayerStack::PushOverlay(std::shared_ptr<Layer> layer)
	{
		PINE_PROFILE_FUNCTION();

		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopOverlay(std::shared_ptr<Layer> layer)
	{
		PINE_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			layer->OnDetach();
		}
	}

}
