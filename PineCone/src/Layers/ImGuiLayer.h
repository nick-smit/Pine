#include <Pine.h>

#include <functional>
#include <vector>

namespace Pine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;

	private:
		std::vector<std::function<void()>> m_UnsubscribeFunctions;
	};

}