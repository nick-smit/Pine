#include "pinepch.h"
#include "Renderer.h"

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Pine\Core\Event.h"

#include <glad\glad.h>

namespace Pine {

	namespace Utils {
		void OpenGLMessageCallback(
			unsigned source,
			unsigned type,
			unsigned id,
			unsigned severity,
			int length,
			const char* message,
			const void* userParam)
		{
			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH:         PINE_LOG_CORE_CRITICAL("OpenGL debug Critical: {0}", message); return;
				case GL_DEBUG_SEVERITY_MEDIUM:       PINE_LOG_CORE_CRITICAL("OpenGL debug Medium: {0}", message); return;
				case GL_DEBUG_SEVERITY_LOW:          PINE_LOG_CORE_WARN("OpenGL debug low: {0}", message); return;
				case GL_DEBUG_SEVERITY_NOTIFICATION: PINE_LOG_CORE_INFO("OpenGL debug notification: {0}", message); return;
			}

			PINE_ASSERT(false, "Unknown severity level!");
		}
	}
	
	void Renderer::Init()
	{
		#ifdef PINE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Utils::OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);

		EventDispatcher<WindowResizeEvent>::Listen([](const WindowResizeEvent& e) {
			RenderCommand::SetViewportSize(e.Width, e.Height);

			return false;
		});

		Renderer2D::Init();
	}

	void Renderer::Terminate()
	{
		Renderer2D::Terminate();
	}

}
