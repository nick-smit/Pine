#pragma once

#include "Scene.h"
#include "Component.h"

#include <entt\entt.hpp>
#include <utility>

namespace Pine {

	class Entity {
	public:
		Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene) {};
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&& ...args) 
		{
			PINE_ASSERT(!HasComponent<T>(), "Entity already has component");
			T& component = m_Scene->GetEnttRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			PINE_ASSERT(HasComponent<T>(), "Entity does not have component");
			m_Scene->GetEnttRegistry().remove<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			PINE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_Scene->GetEnttRegistry().get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->GetEnttRegistry().all_of<T>(m_EntityHandle);
		}

		const std::string& GetTag() const { return m_Scene->GetEnttRegistry().get<TagComponent>(m_EntityHandle).Tag; }


		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene;
	};

}
