#pragma once

#include <string>

namespace Pine {

	class Application {
	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();

	private:
		bool m_Running = false;
	};

}
