#pragma once

#include <memory>
#include <string>

#include <glm\glm.hpp>

namespace Pine {

	class Shader
	{
	public:
		Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~Shader();

		static std::shared_ptr<Shader> FromFile(const std::string& name, const std::string& path);
		static std::shared_ptr<Shader> FromFile(const std::string& name, const std::filesystem::path& path);

		bool IsValid() const;

		void Bind() const;
		void Unbind() const;

		std::string GetName() const { return m_Name; }

		void SetInt(const std::string& name, int32_t value) const;
		void SetIntArray(const std::string& name, int32_t* value, uint32_t count) const;
		void SetUint(const std::string& name, uint32_t value) const;
		void SetUintArray(const std::string& name, uint32_t* value, uint32_t count) const;
		void SetFloat(const std::string& name, float value) const;
		void SetFloat2(const std::string& name, const glm::vec2& value) const;
		void SetFloat3(const std::string& name, const glm::vec3& value) const;
		void SetFloat4(const std::string& name, const glm::vec4& value) const;
		void SetMat3(const std::string& name, const glm::mat3& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	private:
		bool Compile(const std::string& vertexSource, const std::string& fragmentSource);

	private:
		uint32_t m_ProgramId;
		std::string m_Name;
	};

	//TODO(Nick): Shader dictionary?

}
