#pragma once

#include <Pine.h>

#include <filesystem>

namespace Pine {

	struct EntitySelectedEvent
	{
		Entity Entity;

		EntitySelectedEvent() = default;
		EntitySelectedEvent(const ::Pine::Entity& entity)
			: Entity(entity) {};
	};

	struct SceneOpenedEvent
	{
		std::filesystem::path Filepath;

		SceneOpenedEvent() = default;
	};

	struct SceneSavedEvent
	{
		std::filesystem::path Filepath;

		SceneSavedEvent() = default;
	};

}
