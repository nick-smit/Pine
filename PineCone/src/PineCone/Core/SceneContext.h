#pragma once

#include <Pine.h>
#include <memory>

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

	private:
		std::shared_ptr<Scene> m_Context;
	};

}
