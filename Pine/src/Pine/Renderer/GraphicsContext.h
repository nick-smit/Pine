#pragma once

#include "Pine\Core\Window.h"

#include <memory>

namespace Pine {

	class GraphicsContext
	{
	public:
		GraphicsContext(std::shared_ptr<Window> window)
			: m_Window(window) {};
		virtual ~GraphicsContext() = default;

		void Init();
		void SwapBuffers();
		
	private:
		std::shared_ptr<Window> m_Window;
	};

}