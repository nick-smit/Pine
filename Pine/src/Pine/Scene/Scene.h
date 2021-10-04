#pragma once

#include "Pine\Core\Timestep.h"
#include "Pine\Renderer\Camera.h"

#include <entt\entt.hpp>

namespace Pine {

	class Entity;

	class Scene {
	public:
		Scene();
		Scene(const Scene& other) = delete;
		virtual ~Scene();

		Entity CreateEntity(const std::string& name);

		void OnUpdate(Timestep ts, const Camera& camera);

		entt::registry& GetEnttRegistry() { return m_Registry; }

	private:
		entt::registry m_Registry;
	};

}
