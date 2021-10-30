#include "pcpch.h"
#include "ViewportPanel.h"

#include "PineCone\Core\Event.h"
#include "PineCone\ImGui\ErrorPopup.h"
#include "PineCone\ImGui\UI.h"

#include <imgui.h>

#include <string>

namespace Pine {

	void ViewportPanel::OnAttach()
	{
		m_EventListeners.push_back(EventDispatcher<MouseButtonPressedEvent>::Listen([&](const MouseButtonPressedEvent& e) {
			const bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);

			if (!alt && e.Button == MouseButton::Left && m_IsHovered) {
				bool handled = false;

				bool isBound = m_Framebuffer->IsBound();

				m_Framebuffer->Bind();
				
				// Subtract mouse position Y from viewport size Y to negate inverted Y coordinate
				auto px = m_Framebuffer->ReadRedPixel(1, m_WindowSpaceMousePos.x, m_ViewportSize.y - m_WindowSpaceMousePos.y);
				if (px > -1) {
					EventDispatcher<EntitySelectedEvent>::Dispatch({ Entity((entt::entity)px, m_Context->GetContext().get()) });
					handled = true;
				}

				if (!isBound)
					m_Framebuffer->Unbind();

				return handled;
			}

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

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_IsHovered = ImGui::IsWindowHovered();
		m_WindowSpaceMousePos = UI::GetWindowSpaceMousePosition();

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

		bool oldFocusStatus = m_InFocus;
		m_InFocus = ImGui::IsWindowFocused() && m_IsHovered;
		if (m_InFocus != oldFocusStatus) {
			EventDispatcher<ViewportFocusedEvent>::Dispatch({ m_InFocus });
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

}