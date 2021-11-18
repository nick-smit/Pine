#pragma once

#include "Command.h"

#include <Pine.h>

#include <memory>

namespace Pine {

	class HandlesEntities {
	protected:
		HandlesEntities(std::shared_ptr<Scene> scene, Entity entity)
			: m_Scene(scene), m_EntityID(entity.GetID()) {};
		virtual ~HandlesEntities() = default;

		Entity& GetEntity() const
		{
			Entity entity = m_Scene->GetEntityByID(m_EntityID);
			PINE_ASSERT((bool)entity, "Invalid entity!");

			return entity;
		}

		template <typename Component>
		std::shared_ptr<Component> CopyComponentInto(Entity entity)
		{
			if (entity.HasComponent<Component>())
				return std::make_shared<Component>(entity.GetComponent<Component>());

			return nullptr;
		}

		template <typename Component>
		void AddComponentFrom(Entity entity, std::shared_ptr<Component> source)
		{
			if (source)
				entity.AddComponent<Component>(*source.get());
		}
		
	protected:
		std::shared_ptr<Scene> m_Scene;
		Uuid m_EntityID;
	};

	class CreateEntityCommand : public Command, HandlesEntities
	{
	public:
		CreateEntityCommand(std::shared_ptr<Scene> scene)
			: HandlesEntities(scene, Entity()) {};
		virtual ~CreateEntityCommand() = default;

		// Inherited via Command
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Redo() override;
	};

	class DestroyEntityCommand : public Command, HandlesEntities
	{
	private:
	public:
		DestroyEntityCommand(std::shared_ptr<Scene> scene, Entity entity)
			: HandlesEntities(scene, entity) {};
		virtual ~DestroyEntityCommand() = default;

		// Inherited via Command
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Redo() override;
	
	private:
		// Entity components
		std::shared_ptr<TagComponent> m_TagComponent = nullptr;
		std::shared_ptr<TransformComponent> m_TransformComponent = nullptr;
		std::shared_ptr<SpriteRendererComponent> m_SpriteRendererComponent = nullptr;
	};

	template <class Component>
	class AddComponentCommand : public Command, HandlesEntities
	{
	public:
		AddComponentCommand(std::shared_ptr<Scene> scene, Entity entity)
			: HandlesEntities(scene, entity), m_Entity(entity) {};
		virtual ~AddComponentCommand() = default;

		virtual void Execute() override
		{
			m_Entity.AddComponent<Component>();
		}

		virtual void Undo() override
		{
			// We cannot rely on m_Entity anymore because it might have been recreated.
			Entity entity = GetEntity();

			entity.RemoveComponent<Component>();
		}

		virtual void Redo() override
		{
			// We cannot rely on m_Entity anymore because it might have been recreated.
			Entity entity = GetEntity();

			entity.AddComponent<Component>();
		}

	private:
		Entity m_Entity;
	};

	template <class Component>
	class RemoveComponentCommand : public Command, HandlesEntities
	{
	public:
		RemoveComponentCommand(std::shared_ptr<Scene> scene, Entity entity)
			: HandlesEntities(scene, entity), m_Entity(entity) {};
		virtual ~RemoveComponentCommand() = default;

		virtual void Execute() override
		{
			m_RemovedComponent = CopyComponentInto<Component>(m_Entity);
			m_Entity.RemoveComponent<Component>();
		}

		virtual void Undo() override
		{
			// We cannot rely on m_Entity anymore because it might have been recreated.
			Entity entity = GetEntity();

			AddComponentFrom<Component>(entity, m_RemovedComponent);
		}

		virtual void Redo() override
		{
			// We cannot rely on m_Entity anymore because it might have been recreated.
			Entity entity = GetEntity();

			m_RemovedComponent = CopyComponentInto<Component>(entity);
			entity.RemoveComponent<Component>();
		}
	private:
		Entity m_Entity;
		std::shared_ptr<Component> m_RemovedComponent;
	};

	template <class Component>
	class ResetComponentCommand : public Command, HandlesEntities
	{
	public:
		ResetComponentCommand(std::shared_ptr<Scene> scene, Entity entity)
			: HandlesEntities(scene, entity), m_Entity(entity) {};
		virtual ~ResetComponentCommand() = default;

		// Inherited via Command
		virtual void Execute() override
		{
			m_ResetComponent = CopyComponentInto<Component>(m_Entity);
			m_Entity.GetComponent<Component>() = Component();
		}

		virtual void Undo() override
		{
			// We cannot rely on m_Entity anymore because it might have been recreated.
			Entity entity = GetEntity();

			entity.GetComponent<Component>() = *m_ResetComponent.get();
		}

		virtual void Redo() override
		{
			// We cannot rely on m_Entity anymore because it might have been recreated.
			Entity entity = GetEntity();

			m_ResetComponent = CopyComponentInto<Component>(entity);
			entity.GetComponent<Component>() = Component();
		}

	private:
		Entity m_Entity;
		std::shared_ptr<Component> m_ResetComponent = nullptr;
	};
}
