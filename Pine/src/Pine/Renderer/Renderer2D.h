#pragma once

#include "Camera.h"
#include "Texture2D.h"

#include <memory>
#include <glm\glm.hpp>

namespace Pine {

	class Renderer2D {
	public:
		struct QuadSpecification {
			bool UseTransform = false;
			glm::mat4 Transform = glm::mat4(0.0f);

			// Transform
			glm::vec3 Position = glm::vec3(1.0f);
			glm::vec3 Scale = glm::vec3(1.0f);
			float ZRotation = 0.0f;
			
			// Color
			glm::vec4 Color = glm::vec4(1.0f);
			
			// Textures
			float TilingFactor = 1.0f;
			std::shared_ptr<Texture2D> Texture = nullptr;

			QuadSpecification() = default;
			QuadSpecification(const glm::vec3& position)
				: Position(position) {};
			QuadSpecification(const glm::vec3& position, const glm::vec3& scale)
				: Position(position), Scale(scale) {};
			QuadSpecification(const glm::vec3& position, const glm::vec3& scale, float zRotation)
				: Position(position), Scale(scale), ZRotation(zRotation) {};
		};

	public:
		Renderer2D() = delete;
		~Renderer2D() = delete;

		static void Init();
		static void Terminate();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void DrawQuad(const QuadSpecification& spec);
	};

}