#pragma once

#include <Pine.h>
#include <string>

namespace Pine {

	class Panel {
	public:
		Panel(const std::string& name)
			: m_Name(name) {};
		virtual ~Panel() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnRender(Timestep ts) {};

	private:
		std::string m_Name;
	};

}