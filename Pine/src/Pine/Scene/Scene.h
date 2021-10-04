#pragma once

#include "Pine\Core\Timestep.h"
#include "Pine\Renderer\Camera.h"

#include <entt\entt.hpp>

namespace Pine {

	class Scene {
	public:
		Scene();
		Scene(const Scene& other) = delete;
		virtual ~Scene();

		void OnUpdate(Timestep ts, const Camera& camera);

	private:
		entt::registry m_Registry;
	};

}
