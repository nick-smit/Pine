#include <Pine.h>

#include "..\Panels\Panel.h"

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
		std::vector<std::function<void()>> m_UnsubscribeFunctions;

		std::vector<std::shared_ptr<Panel>> m_Panels;
	};

}
