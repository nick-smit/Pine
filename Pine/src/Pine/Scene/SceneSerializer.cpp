#include "pinepch.h"
#include "SceneSerializer.h"

#include "Component.h"
#include "Entity.h"

#include "Pine\Core\File.h"

#include <yaml-cpp\yaml.h>

#define UNABLE_TO_DESERIALIZE(reason) PINE_ASSERT(false, "Unable to deserialize scene: {0}.", reason); return nullptr

YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}
YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.r << v.g << v.b << v.a << YAML::EndSeq;
	return out;
}

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<double>();
			rhs.y = node[1].as<double>();
			rhs.z = node[2].as<double>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4> {
		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<double>();
			rhs.y = node[1].as<double>();
			rhs.z = node[2].as<double>();
			rhs.w = node[3].as<double>();
			return true;
		}
	};
}

namespace Pine {
	
	namespace Utils {

		template<typename T>
		static void SerializeComponent(YAML::Emitter& out, const std::string& name, Entity& entity, std::function<void(T&)> serializeFn)
		{
			if (entity.HasComponent<T>()) {
				out << YAML::Key << name;
				out << YAML::Value << YAML::BeginMap;
		
				serializeFn(entity.GetComponent<T>());
		
				out << YAML::EndMap;
			}
		};

	}

	void SceneSerializer::Serialize(std::shared_ptr<Scene> scene, const std::filesystem::path& filepath)
	{
		PINE_PROFILE_FUNCTION();

		YAML::Emitter out;
		
		out << YAML::BeginMap;
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		
		scene->GetEnttRegistry().each([&](auto entityID) {
			Entity entity = { entityID, scene.get() };
			out << YAML::BeginMap;
			
			out << YAML::Key << "EntityID" << YAML::Value << "123456"; // TODO(Nick): Entity IDs
		
			Utils::SerializeComponent<TagComponent>(out, "TagComponent", entity, [&](auto& component) {
				out << YAML::Key << "Tag" << YAML::Value << component.Tag;
			});
		
			Utils::SerializeComponent<TransformComponent>(out, "TransformComponent", entity, [&](auto& component) {
				out << YAML::Key << "Translation" << YAML::Value << component.Translation;
				out << YAML::Key << "Rotation"    << YAML::Value << component.Rotation;
				out << YAML::Key << "Scale"       << YAML::Value << component.Scale;
			});
		
			Utils::SerializeComponent<SpriteRendererComponent>(out, "SpriteRendererComponent", entity, [&](auto& component) {
				out << YAML::Key << "Color" << YAML::Value << component.Color;
			});
		
			out << YAML::EndMap;
		});
		
		out << YAML::EndSeq;
		out << YAML::EndMap;

		FileStream::PutContents(filepath, out.c_str(), out.size());
	}

	std::shared_ptr<Scene> SceneSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		PINE_PROFILE_FUNCTION();

		YAML::Node sceneData = YAML::LoadFile(filepath.string());

		if (!sceneData["Entities"]) {
			UNABLE_TO_DESERIALIZE("No entities found in scene file");
		}

		YAML::Node entities = sceneData["Entities"];
		if (!entities.IsSequence()) {
			UNABLE_TO_DESERIALIZE("Entities is not a sequence");
		}

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();

		for (YAML::const_iterator it = entities.begin(); it != entities.end(); ++it) {
			YAML::Node curNode = *it;
			if (!curNode.IsMap()) {
				UNABLE_TO_DESERIALIZE("Entity must be a map");
			}

			// Todo entity ID
			if (!curNode["EntityID"]) {
				UNABLE_TO_DESERIALIZE("Entity does not have an ID");
			}

			if (!curNode["TagComponent"]) {
				UNABLE_TO_DESERIALIZE("Entity does not have a TagComponent");
			}
			else if (!curNode["TagComponent"]["Tag"]) {
				UNABLE_TO_DESERIALIZE("Entity does not have a valid TagComponent!");
			}
			Entity entity = scene->CreateEntity(curNode["TagComponent"]["Tag"].as<std::string>());

			if (curNode["TransformComponent"]) {
				PINE_ASSERT(entity.HasComponent<TransformComponent>(), "Entity must be created with a transform component");

				auto& transformComponent = entity.GetComponent<TransformComponent>();
				transformComponent.Translation = curNode["TransformComponent"]["Translation"].as<glm::vec3>();
				transformComponent.Rotation = curNode["TransformComponent"]["Rotation"].as<glm::vec3>();
				transformComponent.Scale = curNode["TransformComponent"]["Scale"].as<glm::vec3>();
			}

			if (curNode["SpriteRendererComponent"]) {
				auto& spriteRendererComponent = entity.AddComponent<SpriteRendererComponent>();

				spriteRendererComponent.Color = curNode["SpriteRendererComponent"]["Color"].as<glm::vec4>();
			}
			
		}
		
		return scene;
	}

}
