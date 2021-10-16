#include "pcpch.h"
#include "EditorPropertiesPanel.h"

#include "PineCone\ImGui\UI.h"

#include <imgui.h>

namespace Pine {
	
	void EditorPropertiesPanel::OnRender(Timestep ts)
	{
		ImGui::Begin("Editor properties");

		ImGuiTreeNodeFlags editorCameraFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
		if (ImGui::TreeNodeEx("Editor camera", editorCameraFlags)) {
			float fov = m_EditorCamera->GetFieldOfView();
			if (UI::DragFloat("FOV", &fov, 0.25f, 0.0f, 180.0f)) {
				m_EditorCamera->SetFieldOfView(fov);
			}

			float zNear = m_EditorCamera->GetZNear();
			if (UI::DragFloat("Near clipping", &zNear)) {
				if (zNear < 0.1f)
					zNear = 0.1f;

				m_EditorCamera->SetZNear(zNear);
			}

			float zFar = m_EditorCamera->GetZFar();
			if (UI::DragFloat("Far clipping", &zFar)) {
				m_EditorCamera->SetZFar(zFar);
			}

			ImGui::Separator();

			auto focalPoint = m_EditorCamera->GetFocalPoint();
			UI::Vec3Control("Focal point", focalPoint);
			if (focalPoint != m_EditorCamera->GetFocalPoint()) {
				m_EditorCamera->SetFocalPoint(focalPoint);
			}

			float distance = m_EditorCamera->GetDistance();
			if (UI::DragFloat("Distance", &distance, 0.1f)) {
				if (distance < 1.0f) {
					distance = 1.0f;
				}
				m_EditorCamera->SetDistance(distance);
			}

			float yaw = m_EditorCamera->GetYaw();
			if (UI::DragFloat("Yaw", &yaw)) {
				m_EditorCamera->SetYaw(yaw);
			}

			float pitch = m_EditorCamera->GetPitch();
			if (UI::DragFloat("Pitch", &pitch)) {
				m_EditorCamera->SetPitch(pitch);
			}
			
			ImGui::TreePop();
		}

		ImGui::End();
	}

}
