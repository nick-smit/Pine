#pragma once

#include "Pine\Core\Timestep.h"
#include "Pine\Core\Uuid.h"
#include "Pine\Renderer\Camera.h"

#include <entt\entt.hpp>

namespace Pine {

	class Entity;

	class Scene {
	public:
		Scene();
		Scene(const Scene& other) = delete;
		virtual ~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const Uuid& id);
		Entity CreateEntity(const Uuid& id, const std::string& name);
		Entity CreateEntity(const std::string& name);
		
		void DestroyEntity(const Entity& entity);

		void OnUpdate(Timestep ts, const Camera& camera);

		Entity GetEntityByID(const Uuid& id);

		entt::registry& GetEnttRegistry() { return m_Registry; }

	private:
		entt::registry m_Registry;
	};

}
