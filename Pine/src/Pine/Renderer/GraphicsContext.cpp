#include "pinepch.h"
#include "GraphicsContext.h"

#include <GLFW\glfw3.h>
#include <glad\glad.h>

namespace Pine {

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_Window->GetNativeWindow());
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PINE_ASSERT(status, "Failed to initialize GLAD!");

		PINE_LOG_CORE_INFO("OpenGL Info:");
		PINE_LOG_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		PINE_LOG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		PINE_LOG_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		PINE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Pine requires at least OpenGL version 4.5!");
	}

	void GraphicsContext::SwapBuffers()
	{
		m_Window->SwapBuffers();
	}

}
