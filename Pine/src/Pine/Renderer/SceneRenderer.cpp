#include "pinepch.h"
#include "SceneRenderer.h"

#include "Renderer2D.h"

#include "Pine\Scene\Entity.h"
#include "Pine\Scene\Component.h"

#include <entt\entt.hpp>

namespace Pine {

	void SceneRenderer::RenderScene(std::shared_ptr<Scene> scene, const Camera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto view = scene->GetEnttRegistry().view<const TransformComponent, const SpriteRendererComponent>();
		view.each([&](const entt::entity& entity, const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer) {
			Renderer2D::QuadSpecification quadSpec(transform.GetTransform());
			quadSpec.Color = spriteRenderer.Color;
			quadSpec.Entity = Entity(entity, scene.get());

			Renderer2D::DrawQuad(quadSpec);
		});

		Renderer2D::EndScene();
	}

}
