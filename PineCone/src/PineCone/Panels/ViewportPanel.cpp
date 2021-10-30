#include "pcpch.h"
#include "ViewportPanel.h"

#include "PineCone\Core\Event.h"
#include "PineCone\ImGui\ErrorPopup.h"
#include "PineCone\ImGui\UI.h"
#include "PineCone\ImGui\UITextureLibrary.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <string>

namespace Pine {

	static float toolbarButtonSize = 24.0f;

	void ViewportPanel::OnAttach()
	{
		m_EventListeners.push_back(EventDispatcher<MouseButtonPressedEvent>::Listen([&](const MouseButtonPressedEvent& e) {
			const bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);

			if (!alt && !ImGuizmo::IsOver() && e.Button == MouseButton::Left && m_IsHovered) {
				bool handled = false;

				bool isBound = m_Framebuffer->IsBound();

				m_Framebuffer->Bind();
				
				// Subtract mouse position Y from viewport size Y to negate inverted Y coordinate
				auto px = m_Framebuffer->ReadRedPixel(1, m_WindowSpaceMousePos.x, m_ViewportSize.y - m_WindowSpaceMousePos.y);
				if (px > -1) {
					auto entity = Entity((entt::entity)px, m_Context->GetContext().get());
					if (entity != m_SelectedEntity) {
						EventDispatcher<EntitySelectedEvent>::Dispatch({ entity });
						handled = true;
					}
				}

				if (!isBound)
					m_Framebuffer->Unbind();

				return handled;
			}

			return false;
		}));

		m_EventListeners.push_back(EventDispatcher<KeyPressedEvent>::Listen([&](const KeyPressedEvent& e) {
			const bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);

			if (!ctrl && m_IsHovered && !ImGuizmo::IsUsing()) {

				switch (e.Key) {
					case Key::Escape:
					case Key::C: {
						m_GuizmoMode = GuizmoMode::Cursor;
						break;
					}
					case Key::M: {
						m_GuizmoMode = GuizmoMode::Translate;
						break;
					}
					case Key::R: {
						m_GuizmoMode = GuizmoMode::Rotate;
						break;
					}
					case Key::S: {
						m_GuizmoMode = GuizmoMode::Scale;
						break;
					}
				}

			}

			return false;
		}));

		m_EventListeners.push_back(EventDispatcher<SceneOpenedEvent>::Listen([&](const SceneOpenedEvent& e) {
			m_SelectedEntity = Entity();

			return false;
		}));

		m_EventListeners.push_back(EventDispatcher<EntitySelectedEvent>::Listen([&](const EntitySelectedEvent& e) {
			m_SelectedEntity = e.Entity;

			return false;
		}));
	}

	void ViewportPanel::OnDetach()
	{
		for (auto unsub : m_EventListeners) {
			unsub();
		}
	}

	void ViewportPanel::OnRender(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		const float toolBarHeight = toolbarButtonSize + ImGui::GetStyle().FramePadding.y * 5;

		ImGuiWindowFlags viewportWindowFlags = 0
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoCollapse;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
		ImGui::Begin("Viewport", NULL, viewportWindowFlags);
		ImGui::PopStyleVar();

		ImGuiWindowFlags childWindowFlags = 0
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_AlwaysUseWindowPadding;
		

		ImGui::BeginChild("Toolbar", { 0, toolBarHeight }, false, childWindowFlags);
		RenderToolbar();
		ImGui::EndChild();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
		ImGui::BeginChild("Viewport", { 0, 0 }, false, childWindowFlags);
		ImGui::PopStyleVar();
		RenderViewport();
		ImGui::EndChild();

		ImGui::End();
	}

	void ViewportPanel::RenderToolbar()
	{
		PINE_PROFILE_FUNCTION();

		GuizmoMode curGuizmoMode = m_GuizmoMode;

		UITextureLibrary* texLib = UITextureLibrary::Get();

		const ImVec2 buttonSize = { toolbarButtonSize, toolbarButtonSize };
		
		auto& style = ImGui::GetStyle();
		auto origButtonCol = style.Colors[ImGuiCol_Button];

		ImVec4 activeColor = style.Colors[ImGuiCol_ButtonActive];
		ImVec4 defaultColor = ImVec4(0, 0, 0, 0);

		style.Colors[ImGuiCol_Button] = defaultColor;

		if (curGuizmoMode == GuizmoMode::Cursor) style.Colors[ImGuiCol_Button] = activeColor;
		if (ImGui::ImageButton((void*)texLib->GetTextureID(UITexture::CursorFill), buttonSize, { 0,0 }, { 1,1 }, 0)) {
			m_GuizmoMode = GuizmoMode::Cursor;
		}
		if (curGuizmoMode == GuizmoMode::Cursor) style.Colors[ImGuiCol_Button] = defaultColor;
		ImGui::SameLine();

		if (curGuizmoMode == GuizmoMode::Translate) style.Colors[ImGuiCol_Button] = activeColor;
		if (ImGui::ImageButton((void*)texLib->GetTextureID(UITexture::TranslateFill), buttonSize, { 0,0 }, { 1,1 }, 0)) {
			m_GuizmoMode = GuizmoMode::Translate;
		}
		if (curGuizmoMode == GuizmoMode::Translate) style.Colors[ImGuiCol_Button] = defaultColor;
		ImGui::SameLine();

		if (curGuizmoMode == GuizmoMode::Rotate) style.Colors[ImGuiCol_Button] = activeColor;
		if (ImGui::ImageButton((void*)texLib->GetTextureID(UITexture::RotateFill), buttonSize, { 0,0 }, { 1,1 }, 0)) {
			m_GuizmoMode = GuizmoMode::Rotate;
		}
		if (curGuizmoMode == GuizmoMode::Rotate) style.Colors[ImGuiCol_Button] = defaultColor;
		ImGui::SameLine();

		if (curGuizmoMode == GuizmoMode::Scale) style.Colors[ImGuiCol_Button] = activeColor;
		if (ImGui::ImageButton((void*)texLib->GetTextureID(UITexture::ScaleFill), buttonSize, { 0,0 }, { 1,1 }, 0)) {
			m_GuizmoMode = GuizmoMode::Scale;
		}
		if (curGuizmoMode == GuizmoMode::Scale) style.Colors[ImGuiCol_Button] = defaultColor;
		ImGui::SameLine();

		style.Colors[ImGuiCol_Button] = origButtonCol;
	}

	void ViewportPanel::RenderViewport()
	{
		PINE_PROFILE_FUNCTION();

		m_IsHovered = ImGui::IsWindowHovered();
		m_WindowSpaceMousePos = UI::GetWindowSpaceMousePosition();

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		glm::vec2 viewportBoundsMin = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		glm::vec2 viewportBoundsMax = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureId = m_Framebuffer->GetColorAttachmentId();
		ImGui::Image(reinterpret_cast<void*>(textureId), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file")) {
				const std::filesystem::path path = *(const std::filesystem::path*)payload->Data;

				if (path.extension().string() == ".pinescene") {
					EventDispatcher<SceneOpenedEvent>::Dispatch({ path });
				}
				else {
					ErrorPopup::Open(fmt::format("Cannot open '{0}' in the viewport", path.filename().string()));
				}
			}

			ImGui::EndDragDropTarget();
		}

		// ImGuizmo
		if (m_SelectedEntity && m_GuizmoMode != GuizmoMode::Cursor) {
			PINE_PROFILE_SCOPE("ImGuizmo");

			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(viewportBoundsMin.x, viewportBoundsMin.y, viewportBoundsMax.x - viewportBoundsMin.x, viewportBoundsMax.y - viewportBoundsMin.y);

			if (Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt)) {
				ImGuizmo::Enable(false);
			}
			else {
				ImGuizmo::Enable(true);
			}

			const glm::mat4& cameraProjection = m_Camera->GetCamera().GetProjectionMatrix();
			const glm::mat4& cameraView = m_Camera->GetCamera().GetViewMatrix();

			auto& transformComponent = m_SelectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();

			ImGuizmo::OPERATION imGuizmoOperation;
			switch (m_GuizmoMode) {
				case GuizmoMode::Translate: imGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE; break;
				case GuizmoMode::Rotate:    imGuizmoOperation = ImGuizmo::OPERATION::ROTATE; break;
				case GuizmoMode::Scale:     imGuizmoOperation = ImGuizmo::OPERATION::SCALE; break;
				default: PINE_ASSERT(false, "Unknown guizmo mode");
			}

			const bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
			float snapping = 0.0f;
			if (ctrl) {
				snapping = 0.1f;

				if (m_GuizmoMode == GuizmoMode::Rotate) {
					snapping = 45.0f;
				}
			}

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), imGuizmoOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr, snapping ? &snapping : nullptr);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				transformComponent.Translation = translation;
				transformComponent.Rotation = rotation;
				transformComponent.Scale = scale;
			}
		}

		bool oldFocusStatus = m_InFocus;
		m_InFocus = ImGui::IsWindowFocused() && m_IsHovered;
		if (m_InFocus != oldFocusStatus) {
			EventDispatcher<ViewportFocusedEvent>::Dispatch({ m_InFocus });
		}
	}

}