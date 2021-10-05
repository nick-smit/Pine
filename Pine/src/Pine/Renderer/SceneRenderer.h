#pragma once

#include "Pine\Scene\Scene.h"
#include "Camera.h"

#include <memory>

namespace Pine {

	class SceneRenderer
	{
	public:
		static void RenderScene(std::shared_ptr<Scene> scene, const Camera& camera);
	};

}
