#pragma once

#include <Pine.h>
#include <string>

namespace Pine {

	class Panel {
	public:
		Panel() = default;
		Panel(const Panel& other) = delete;
		virtual ~Panel() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnRender(Timestep ts) {};


	public:
		static std::string GetName() { PINE_ASSERT(false, "GetName must be implemented by child class"); return "Unnamed Panel"; };
	};

}