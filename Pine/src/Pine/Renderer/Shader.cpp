#include "pinepch.h"
#include "Shader.h"

#include <glad\glad.h>
#include <glm\gtc\type_ptr.hpp>

namespace Pine {

	Shader::Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		m_ProgramId = glCreateProgram();
		bool success = Compile(vertexSource, fragmentSource);
		if (!success) {
			glDeleteProgram(m_ProgramId);
			m_ProgramId = 0;
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramId);
	}

	bool Shader::IsValid() const
	{
		return m_ProgramId != 0;
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ProgramId);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int32_t value) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniform1i(location, value);
	}

	void Shader::SetIntArray(const std::string& name, int32_t* value, uint32_t count) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniform1iv(location, count, value);
	}

	void Shader::SetUint(const std::string& name, uint32_t value) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniform1ui(location, value);
	}

	void Shader::SetUintArray(const std::string& name, uint32_t* value, uint32_t count) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniform1uiv(location, count, value);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniform1f(location, value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniform2f(location, value.x, value.y);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& value) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		PINE_ASSERT((location != -1), "Unable to find location for uniform {0} in shader {1}", name, m_Name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	bool Shader::Compile(const std::string& vertexSource, const std::string& fragmentSource)
	{
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

		{
			auto cstr = vertexSource.c_str();
			glShaderSource(vertexShader, 1, &cstr, NULL);
		}


		glCompileShader(vertexShader);

		{
			int success;
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				char infoLog[512];
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				PINE_ASSERT(success, "Unable to compile vertex shader:\n{0}", infoLog);

				return false;
			}
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		{
			auto cstr = fragmentSource.c_str();
			glShaderSource(fragmentShader, 1, &cstr, NULL);
		}

		glCompileShader(fragmentShader);

		{
			// Test for success
			int success;
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				char infoLog[512];
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				PINE_ASSERT(success, "Unable to compile fragment shader:\n{0}", infoLog);

				return false;
			}
		}

		glAttachShader(m_ProgramId, vertexShader);
		glAttachShader(m_ProgramId, fragmentShader);
		glLinkProgram(m_ProgramId);

		{
			// Test for success
			int success;
			glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &success);
			if (!success) {
				char infoLog[512];
				glGetProgramInfoLog(m_ProgramId, 512, NULL, infoLog);
				PINE_ASSERT(success, "Unable to link shader\n{0}", infoLog);

				return false;
			}
		}

		glUseProgram(m_ProgramId);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return true;
	}

}
