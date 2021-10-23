#include "pcpch.h"
#include "ContentBrowserPanel.h"

#include "PineCone\ImGui\UI.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Pine {

	void ContentBrowserPanel::OnAttach()
	{
		// Todo(Nick): Project directories
		m_BaseDirectory = std::filesystem::current_path();
		m_CurrentDirectory = m_BaseDirectory / "assets";

		m_UITextureLibrary = UITextureLibrary::Get();
	}

	void ContentBrowserPanel::OnDetach()
	{
		m_UITextureLibrary = nullptr;
	}

	void ContentBrowserPanel::OnRender(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Content browser");
		ImGui::PopStyleVar();

		{
			ImGui::BeginChild("Tree view", ImVec2(200.0f, 0.0f));

			ImGui::Text("Todo: directory tree");

			ImGui::EndChild(); // End tree view
		}
		
		ImGui::SameLine();

		RenderNodeView();

		ImGui::End();
	}

	void ContentBrowserPanel::RenderNodeView()
	{
		PINE_PROFILE_FUNCTION();

		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::BeginGroup();

		glm::vec2 actionButtonSize = { 20.0f, 20.0f };
		float actionBarPadding = (float)style.WindowPadding.y;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.WindowPadding.x, actionBarPadding / 2.0f));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, UI::GetColor("ActionBarBg"));
		ImGui::BeginChild("Action bar", ImVec2(0.0f, actionBarPadding * 2 + actionButtonSize.y), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			if (UI::ImageButton(UITexture::ArrowLeft, actionButtonSize, m_PreviousDirectory.empty())) {
				GotoPrevious();
			}

			ImGui::SameLine();

			if (UI::ImageButton(UITexture::ArrowRight, actionButtonSize, m_NextDirectory.empty())) {
				GotoNext();
			}

			ImGui::SameLine();

			if (UI::ImageButton(UITexture::ArrowUp, actionButtonSize, m_CurrentDirectory.parent_path() == m_BaseDirectory)) {
				Goto(m_CurrentDirectory.parent_path());
			}

			ImGui::SameLine();

			auto relativePath = std::filesystem::relative(m_CurrentDirectory, m_BaseDirectory);
			std::filesystem::path clickedPath = m_BaseDirectory;
			for (const auto& dirname : relativePath) {
				clickedPath /= dirname;

				if (UI::Button(dirname.string(), { 0.0f, actionButtonSize.y })) {
					Goto(clickedPath);
				}
				
				ImGui::SameLine();
			}
		} ImGui::EndChild(); // End action bar
		ImGui::PopStyleColor(); // ActionBarBg
		ImGui::PopStyleVar(); // WindowPadding

		ImGui::BeginChild("Node view");

		size_t totalNodes = std::distance(std::filesystem::directory_iterator(m_CurrentDirectory), std::filesystem::directory_iterator{});

		uint32_t nodeIndex = 0;
		float windowVisiblex2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		ImVec2 nodeSize = ImVec2(96, 96);
		ImVec4 iconColor = UI::GetColor("Icon_Default");

		for (const auto& node : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			ImGui::PushID(nodeIndex);

			if (node.is_directory()) {
				ImGui::BeginGroup();
				ImGui::Image((void*)m_UITextureLibrary->GetTextureID(UITexture::Folder), nodeSize, ImVec2(0, 0), ImVec2(1, 1), iconColor);
				UI::TextCentered(node.path().stem().string(), (float)nodeSize.x);
				ImGui::EndGroup();

				if (ImGui::IsItemHovered()) {
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						Goto(node.path());
					}
				}
			}
			else {
				ImGui::BeginGroup();
				ImGui::Image((void*)m_UITextureLibrary->GetTextureID(UITexture::File), nodeSize, ImVec2(0, 0), ImVec2(1, 1), iconColor);
				UI::TextCentered(node.path().filename().string(), (float)nodeSize.x);
				ImGui::EndGroup();
			}

			float lastNodeX2 = ImGui::GetItemRectMax().x;
			float nextNodeX2 = lastNodeX2 + style.ItemSpacing.x + nodeSize.x; // Expected position if next button was on same line
			if (++nodeIndex < totalNodes && nextNodeX2 < windowVisiblex2)
				ImGui::SameLine();

			ImGui::PopID();
		}

		ImGui::EndChild(); // End node view

		ImGui::EndGroup();
	}

	void ContentBrowserPanel::GotoPrevious()
	{
		std::filesystem::path path = m_PreviousDirectory.back();
		Goto(path, GotoAction::Previous);
	}

	void ContentBrowserPanel::GotoNext()
	{
		std::filesystem::path path = m_NextDirectory.back();
		Goto(path, GotoAction::Next);
	}

	void ContentBrowserPanel::Goto(const std::filesystem::path& path, GotoAction action)
	{
		if (path == m_CurrentDirectory)
			return;

		if (action == GotoAction::Previous) {
			m_NextDirectory.push_back(m_CurrentDirectory);
			m_PreviousDirectory.pop_back();
		}
		else {
			m_PreviousDirectory.push_back(m_CurrentDirectory);

			if (action != GotoAction::Next) {
				m_NextDirectory.clear();
			}
			else {
				m_NextDirectory.pop_back();
			}
		}

		m_CurrentDirectory = path;
	}

}
