#pragma once

#include "Timestep.h"

#include <string>

namespace Pine {

	class Layer {
	public:
		Layer(const std::string& name)
			: m_Name(name) {}
		Layer(const Layer& other) = delete;
		virtual ~Layer() = default;

	public:
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(Timestep ts) = 0;

		const std::string& GetName() const { return m_Name; }

	private:
		std::string m_Name;
	};

}
