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

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TagComponent>(name.empty() ? "Empty Entity" : name);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts, const Camera& camera)
	{
	}

}
