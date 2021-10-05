#pragma once

#include <Pine.h>

namespace Pine {

	struct EntitySelectedEvent
	{
		Entity Entity;

		EntitySelectedEvent() = default;
		EntitySelectedEvent(const ::Pine::Entity& entity)
			: Entity(entity) {};
	};

}
