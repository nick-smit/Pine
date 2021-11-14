#include "pinepch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"

namespace Pine {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity()
	{
		PINE_PROFILE_FUNCTION();
		
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>();
		
		return entity;
	}

	Entity Scene::CreateEntity(const Uuid& id)
	{
		PINE_PROFILE_FUNCTION();

		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(id);

		return entity;
	}

	Entity Scene::CreateEntity(const Uuid& id, const std::string& name)
	{
		PINE_PROFILE_FUNCTION();

		Entity entity = CreateEntity(id);
		entity.AddComponent<TagComponent>(name.empty() ? "Empty Entity" : name);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		PINE_PROFILE_FUNCTION();

		Entity entity = CreateEntity();
		entity.AddComponent<TagComponent>(name.empty() ? "Empty Entity" : name);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		PINE_PROFILE_FUNCTION();

		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts, const Camera& camera)
	{
		PINE_PROFILE_FUNCTION();

	}

	Entity Scene::GetEntityByID(const Uuid& id)
	{
		PINE_PROFILE_FUNCTION();

		// TODO(Nick): Might need optimization
		auto view = m_Registry.view<IDComponent>();
		for (auto& entityId : view) {
			Entity entity = { entityId, this };
			if (entity.GetID() == id)
				return entity;
		}

		return Entity();
	}

}
