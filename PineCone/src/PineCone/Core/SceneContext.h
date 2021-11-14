#pragma once

#include <Pine.h>
#include <memory>
#include <filesystem>

namespace Pine {

	class SceneContext
	{
	public:
		SceneContext() = default;
		SceneContext(std::shared_ptr<Scene> context)
			: m_Context(context) {};

		void SetContext(std::shared_ptr<Scene> context) { m_Context = context; }
		std::shared_ptr<Scene> GetContext() const { 
			PINE_ASSERT(m_Context, "Scene context must be set!");
			return m_Context; 
		}

		void SetPath(const std::filesystem::path& path) { m_CurrentScenePath = path; }
		const std::filesystem::path& GetPath() const { return m_CurrentScenePath; }

		void SetPristine(bool pristine) { m_Pristine = pristine; }
		bool IsPristine() const { return m_Pristine; }

	private:
		std::filesystem::path m_CurrentScenePath = {};
		bool m_Pristine = true;
		std::shared_ptr<Scene> m_Context;
	};

}
