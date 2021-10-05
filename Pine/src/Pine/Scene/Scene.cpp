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

		return entity;
	}

	void Scene::OnUpdate(Timestep ts, const Camera& camera)
	{
	}

}
