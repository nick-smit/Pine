#include <Pine.h>

#include "PineCone\Core\PanelManager.h"
#include "PineCone\Panels\Panel.h"

#include <functional>
#include <memory>
#include <vector>

namespace Pine {

	class PineConeLayer : public Layer
	{
	public:
		PineConeLayer();
		virtual ~PineConeLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;

	private:
		void BeginDockspace() const;
		void EndDockspace() const;
	private:
		std::vector<std::function<void()>> m_UnsubscribeFunctions;

		PanelManager m_panelManager;
	};

}
