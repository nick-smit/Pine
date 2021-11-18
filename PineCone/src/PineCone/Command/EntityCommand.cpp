#include "pcpch.h"
#include "EntityCommand.h"
#include "PineCone\Core\Event.h"

namespace Pine {
	
	/// 
	/// Create entity
	/// 
	
	void CreateEntityCommand::Execute()
	{
		Entity entity = m_Scene->CreateEntity("Empty Entity");
		m_EntityID = entity.GetID();

		EventDispatcher<EntitySelectedEvent>::Dispatch({ entity });
	}

	void CreateEntityCommand::Undo()
	{
		Entity entity = GetEntity();

		EventDispatcher<EntityDestroyedEvent>::Dispatch({ entity });
		m_Scene->DestroyEntity(entity);
	}

	void CreateEntityCommand::Redo()
	{
		Entity entity = m_Scene->CreateEntity(m_EntityID, "Empty Entity");
	}


	/// 
	/// Destroy entity
	/// 

	void DestroyEntityCommand::Execute()
	{
		// TODO(Nick): Figure out why this crashes
		Entity entity = GetEntity();

		m_TagComponent = CopyComponentInto<TagComponent>(entity);
		m_TransformComponent = CopyComponentInto<TransformComponent>(entity);
		m_SpriteRendererComponent = CopyComponentInto<SpriteRendererComponent>(entity);

		EventDispatcher<EntityDestroyedEvent>::Dispatch({ entity });
		m_Scene->DestroyEntity(entity);
	}

	void DestroyEntityCommand::Undo()
	{
		Entity entity = m_Scene->CreateEntity(m_EntityID);

		AddComponentFrom<TagComponent>(entity, m_TagComponent);
		AddComponentFrom<TransformComponent>(entity, m_TransformComponent);
		AddComponentFrom<SpriteRendererComponent>(entity, m_SpriteRendererComponent);
	}

	void DestroyEntityCommand::Redo()
	{
		Execute();
	}

}
