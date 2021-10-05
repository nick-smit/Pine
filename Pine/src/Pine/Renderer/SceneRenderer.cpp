#include "pinepch.h"
#include "SceneRenderer.h"

#include "Renderer2D.h"

#include "Pine\Scene\Component.h"

namespace Pine {

	void SceneRenderer::RenderScene(std::shared_ptr<Scene> scene, const Camera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto view = scene->GetEnttRegistry().view<const TransformComponent, const SpriteRendererComponent>();
		view.each([](const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer) {
			Renderer2D::QuadSpecification quadSpec(transform.GetTransform());
			quadSpec.Color = spriteRenderer.Color;

			Renderer2D::DrawQuad(quadSpec);
		});

		Renderer2D::EndScene();
	}

}
