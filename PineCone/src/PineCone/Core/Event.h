#pragma once

#include <Pine.h>

#include <filesystem>

namespace Pine {

	struct EntitySelectedEvent
	{
		Entity Entity;

		EntitySelectedEvent() = default;
	};

	struct EntityDestroyedEvent
	{
		Entity Entity;

		EntityDestroyedEvent() = default;
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

	struct SceneChangedEvent
	{
		SceneChangedEvent() = default;
	};

	struct ViewportFocusedEvent {
		bool Status;

		ViewportFocusedEvent() = default;
	};

}
