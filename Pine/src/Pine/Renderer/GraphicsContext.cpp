#include "pinepch.h"
#include "GraphicsContext.h"

#include <GLFW\glfw3.h>
#include <glad\glad.h>

namespace Pine {

	void GraphicsContext::Init()
	{
		PINE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_Window->GetNativeWindow());
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PINE_ASSERT(status, "Failed to initialize GLAD!");

		PINE_LOG_CORE_INFO("OpenGL Info:");
		PINE_LOG_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		PINE_LOG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		PINE_LOG_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		PINE_ASSERT((GLVersion.major > PINE_OPENGL_MAJOR_VERSION || (GLVersion.major == PINE_OPENGL_MAJOR_VERSION && GLVersion.minor >= PINE_OPENGL_MINOR_VERSION)), "Pine requires at least OpenGL version 4.5!");
	}

	void GraphicsContext::SwapBuffers()
	{
		m_Window->SwapBuffers();
	}

}
