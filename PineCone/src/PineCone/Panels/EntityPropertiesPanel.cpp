#include "pcpch.h"
#include "EntityPropertiesPanel.h"
#include "PineCone\Core\Event.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm\glm.hpp>

namespace Pine {

	namespace Utils {

		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(label.c_str());

			PC_IMGUI_BEGIN_INPUT_WIDGET_COLUMNS();

			ImGui::Text(label.c_str());

			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.17f, 0.6f, 0.17f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.7f, 0.25f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.17f, 0.6f, 0.17f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			PC_IMGUI_END_INPUT_WIDGET_COLUMNS();

			ImGui::PopID();
		}

	}

	void EntityPropertiesPanel::OnAttach()
	{
		m_EventListeners.push_back(EventDispatcher<EntitySelectedEvent>::Listen([&](const EntitySelectedEvent& e) {
			m_SelectedEntity = e.Entity;

			return false;
		}));
	}

	void EntityPropertiesPanel::OnDetach()
	{
		for (auto unsub : m_EventListeners) {
			unsub();
		}
	}

	void EntityPropertiesPanel::OnRender(Timestep ts)
	{
		ImGui::Begin("Properties");

		if (m_SelectedEntity)
			// DrawTagComponent might destroy the selected entity
			DrawTagComponent();

		if (m_SelectedEntity) {
			DrawComponent<TransformComponent>("Transform", [](TransformComponent& transformComponent) {
				float speed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl) ? 0.01f : 0.1f;

				Utils::DrawVec3Control("Translation", transformComponent.Translation);
				
				glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
				Utils::DrawVec3Control("Rotation", rotation);
				transformComponent.Rotation = glm::radians(rotation);
				
				Utils::DrawVec3Control("Scale", transformComponent.Scale, 1.0f);
			});
			
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", [](SpriteRendererComponent& spriteRenderer) {
				PC_IMGUI_BEGIN_INPUT_WIDGET_COLUMNS();

				ImGui::Text("Color");
				ImGui::NextColumn();
				ImGui::ColorEdit4("", &spriteRenderer.Color.r, ImGuiColorEditFlags_NoLabel);

				PC_IMGUI_END_INPUT_WIDGET_COLUMNS();
			});
		}

		ImGui::End();
	}

	void EntityPropertiesPanel::DrawTagComponent()
	{
		PINE_ASSERT(m_SelectedEntity.HasComponent<TagComponent>(), "Entity without tagcomponent!");

		if (m_SelectedEntity.HasComponent<TagComponent>())
		{
			auto& tag = m_SelectedEntity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

			PC_IMGUI_SAME_LINE_SPACE_BETWEEN_TEXT("...");
			if (ImGui::Button("...")) {
				ImGui::OpenPopup("Add component popup");
			}
			if (ImGui::BeginPopup("Add component popup")) {
				if (!m_SelectedEntity.HasComponent<TransformComponent>()) {
					if (ImGui::MenuItem("Transform")) {
						m_SelectedEntity.AddComponent<TransformComponent>();
					}
				}

				if (!m_SelectedEntity.HasComponent<SpriteRendererComponent>()) {
					if (ImGui::MenuItem("Sprite Renderer")) {
						m_SelectedEntity.AddComponent<SpriteRendererComponent>();
					}
				}

				if (m_SelectedEntity.HasAllComponents<TransformComponent, SpriteRendererComponent>()) {
					ImGui::MenuItem("No components to add", NULL, false, false);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Remove Entity")) {
					m_SceneContext->GetContext()->DestroyEntity(m_SelectedEntity);
					EventDispatcher<EntitySelectedEvent>::Dispatch({ Entity() });
				}

				ImGui::EndPopup();
			}

		}
	}
}
