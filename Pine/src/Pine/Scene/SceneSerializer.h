#pragma once

#include "Scene.h"

#include <memory>
#include <filesystem>

namespace Pine {

	class SceneSerializer {
	public:
		static void Serialize(std::shared_ptr<Scene> scene, const std::filesystem::path& filepath);
		static std::shared_ptr<Scene> Deserialize(const std::filesystem::path& filepath);

	private:
		
	};

}
