#include "pcpch.h"
#include "ContentBrowserPanel.h"

#include "PineCone\ImGui\UI.h"
#include "PineCone\ImGui\ErrorPopup.h"
#include "PineCone\Platform\FileDialog.h"
#include "PineCone\Platform\ShellExec.h"

#include <imgui.h>
#include <imgui_internal.h>

/// <summary>
/// Wanted features
/// * Move directories/files
/// * Rename directories/files
/// * Open files in external editor
/// * Project file
/// </summary>

static std::filesystem::path dragDropPayload;

namespace Pine {
	ContentBrowserPanel::ContentBrowserPanel()
	{
		memset(m_ModalInputBuffer, 0, sizeof(m_ModalInputBuffer));
	}

	void ContentBrowserPanel::OnAttach()
	{
		// Todo(Nick): Project directories
		m_BaseDirectory = std::filesystem::current_path() / "assets";
		m_CurrentDirectory = m_BaseDirectory;

		m_UITextureLibrary = UITextureLibrary::Get();

		m_EventListeners.push_back(EventDispatcher<MouseButtonReleasedEvent>::Listen([&](const MouseButtonReleasedEvent& e) {
			if (m_ContentBrowserPanelFocused && m_ContentBrowserPanelHovered) {
				switch (e.Button) {
					case MouseButton::Four: {
						GotoPrevious();
						return true;
					}
					case MouseButton::Five: {
						GotoNext();
						return true;
					}
				}
			}

			return false;
		}));
	}

	void ContentBrowserPanel::OnDetach()
	{
		for (auto& unsub : m_EventListeners) {
			unsub();
		}

		m_UITextureLibrary = nullptr;
	}

	void ContentBrowserPanel::OnRender(Timestep ts)
	{
		PINE_PROFILE_FUNCTION();

		bool baseDirMissing = !std::filesystem::exists(m_BaseDirectory);

		if (!std::filesystem::exists(m_CurrentDirectory) && !baseDirMissing) {
			ErrorPopup::Open(fmt::format("Currently opened directory {0} does not exists.", m_CurrentDirectory.string()));
			m_CurrentDirectory = m_BaseDirectory;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Content browser");

		m_ContentBrowserPanelFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy);
		m_ContentBrowserPanelHovered = ImGui::IsWindowHovered(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy);

		ImGui::PopStyleVar();

		if (baseDirMissing) {
			ImGui::OpenPopup("Base directory error");
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Base directory error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			// TODO(Nick): Use base dir of project file
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 350.0f);
			ImGui::Text("The base directory does not exist anymore. It might have been moved or deleted externally.");
			ImGui::Text("Please select a new directory");
			ImGui::Separator();
			if (UI::Button("Select directory...", { 0, 0 })) {
				std::filesystem::path result;
				while (result.empty()) {
					result = FileDialog::SelectDirectory();
				}
			
				m_CurrentDirectory = m_BaseDirectory = result;
				// Todo(Nick): Reload assets

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (baseDirMissing) {
			ImGui::Text("Base directory is missing!");
			ImGui::End();
			return;
		}
		
		RenderTreeView();
		
		ImGui::SameLine();

		RenderNodeView();

		ImGui::End();
	}

	void ContentBrowserPanel::RenderTreeView()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.WindowPadding);
		ImGui::BeginChild("Tree view", ImVec2(200.0f, 0.0f)); {

			RenderDirectoryTree(m_BaseDirectory);

		} ImGui::EndChild(); // End tree view
		ImGui::PopStyleVar();
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

			if (UI::ImageButton(UITexture::ArrowUp, actionButtonSize, m_CurrentDirectory == m_BaseDirectory)) {
				Goto(m_CurrentDirectory.parent_path());
			}

			ImGui::SameLine();

			auto relativePath = std::filesystem::relative(m_CurrentDirectory, m_BaseDirectory);
			
			if (UI::Button(m_BaseDirectory.filename().string().c_str(), { 0.0f, actionButtonSize.y })) {
				Goto(m_BaseDirectory);
			}
			
			if (m_CurrentDirectory != m_BaseDirectory) {
				ImGui::SameLine();

				std::filesystem::path clickedPath = m_BaseDirectory;
				for (const auto& dirname : relativePath) {
					clickedPath /= dirname;

					if (UI::Button(dirname.string(), { 0.0f, actionButtonSize.y })) {
						Goto(clickedPath);
					}

					ImGui::SameLine();
				}
			}
		} ImGui::EndChild(); // End action bar
		ImGui::PopStyleColor(); // ActionBarBg
		ImGui::PopStyleVar(); // WindowPadding

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.8f, 0.2f, 0.1f, 1.0f));
		ImGui::BeginChild("Node view");

		std::string openPopup;
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("Show in Explorer")) {
				ShellExec::Open(m_CurrentDirectory);
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("New")) {
				if (ImGui::MenuItem("Directory")) {
					openPopup = "New directory";
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		if (!openPopup.empty()) {
			ImGui::OpenPopup(openPopup.c_str());
		}
		
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		// Todo nick: Figure out why the popup status is not open?
		if (ImGui::BeginPopupModal("New directory", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			auto closeFn = [&]() {
				memset(m_ModalInputBuffer, 0, sizeof(m_ModalInputBuffer));
				ImGui::CloseCurrentPopup();
			};

			ImGui::Text("Please enter the name for the new directory");
			
			char buffer[255];
			strcpy_s(buffer, sizeof(buffer), m_ModalInputBuffer);
			if (ImGui::InputText("##dirname", buffer, sizeof(buffer))) {
				strcpy_s(m_ModalInputBuffer, sizeof(m_ModalInputBuffer), buffer);
			}

			if (ImGui::IsItemFocused() && Input::IsKeyPressed(Key::Enter)) {
				MakeDir(m_ModalInputBuffer);
				closeFn();
			}
			
			if (UI::Button("Cancel", { 120, 0 })) {
				closeFn();
			}
			ImGui::SameLine();
			if (UI::Button("Create directory", { 120, 0 })) {
				MakeDir(m_ModalInputBuffer);
				closeFn();
			}
			
			ImGui::EndPopup();
		}

		uint32_t nodeIndex = 0;
		size_t totalNodes = std::distance(std::filesystem::directory_iterator(m_CurrentDirectory), std::filesystem::directory_iterator{});

		float windowVisiblex2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		ImVec2 nodeSize = ImVec2(96, 96);
		ImVec4 iconColor = UI::GetColor("Icon_Default");

		for (const auto& node : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const std::string& fn = node.path().filename().string();
			
			ImGui::PushID(fn.c_str());

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

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					dragDropPayload = node.path();
					ImGui::SetDragDropPayload("file", &dragDropPayload, sizeof(dragDropPayload));

					ImGui::Text(node.path().filename().string().c_str());
					
					ImGui::EndDragDropSource();
				}
			}

			if (ImGui::BeginPopupContextItem(fn.c_str())) {
				if (ImGui::MenuItem("Show in Explorer")) {
					ShellExec::Open(m_CurrentDirectory);
				}

				ImGui::EndPopup();
			}

			float lastNodeX2 = ImGui::GetItemRectMax().x;
			float nextNodeX2 = lastNodeX2 + style.ItemSpacing.x + nodeSize.x; // Expected position if next button was on same line
			if (++nodeIndex < totalNodes && nextNodeX2 < windowVisiblex2)
				ImGui::SameLine();

			ImGui::PopID();
		}

		ImGui::EndChild(); // End node view
		ImGui::PopStyleColor();

		ImGui::EndGroup();
	}

	void ContentBrowserPanel::RenderDirectoryTree(const std::filesystem::path& path)
	{
		PINE_PROFILE_FUNCTION();

		for (const auto& directory : std::filesystem::directory_iterator(path)) {
			if (!directory.is_directory())
				continue;

			ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

			if (directory == m_CurrentDirectory)
				baseFlags |= ImGuiTreeNodeFlags_Selected;

			bool hasSubDirectories = false;
			for (const auto& subDir : std::filesystem::directory_iterator(directory)) {
				if (subDir.is_directory()) {
					hasSubDirectories = true;
					break;
				}
			}
			if (hasSubDirectories)
				baseFlags &= ~ImGuiTreeNodeFlags_Leaf;

			bool open = ImGui::TreeNodeEx(directory.path().filename().string().c_str(), baseFlags);
			if (ImGui::IsItemClicked()) {
				Goto(directory);
			}

			if (open) {
				RenderDirectoryTree(directory);

				ImGui::TreePop();
			}

		}
	}

	void ContentBrowserPanel::GotoPrevious()
	{
		if (m_PreviousDirectory.empty())
			return;

		std::filesystem::path path = m_PreviousDirectory.back();
		Goto(path, GotoAction::Previous);
	}

	void ContentBrowserPanel::GotoNext()
	{
		if (m_NextDirectory.empty())
			return;

		std::filesystem::path path = m_NextDirectory.back();
		Goto(path, GotoAction::Next);
	}

	void ContentBrowserPanel::Goto(const std::filesystem::path& path, GotoAction action)
	{
		if (path == m_CurrentDirectory)
			return;

		if (!std::filesystem::exists(path)) {
			ErrorPopup::Open(fmt::format("Directory {0} does not exist.", path.string()));

			if (action == GotoAction::Previous) {
				m_PreviousDirectory.pop_back();
			}
			else if (action == GotoAction::Next) {
				m_NextDirectory.pop_back();
			}

			return;
		}

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

	bool ContentBrowserPanel::MakeDir(const char* buffer)
	{
		if (   std::strstr(buffer, ":")  != nullptr 
			|| std::strstr(buffer, "<")  != nullptr
			|| std::strstr(buffer, ">")  != nullptr
			|| std::strstr(buffer, "\"") != nullptr
			|| std::strstr(buffer, "/")  != nullptr
			|| std::strstr(buffer, "\\") != nullptr
			|| std::strstr(buffer, "|")  != nullptr
			|| std::strstr(buffer, "?")  != nullptr
			|| std::strstr(buffer, "*")  != nullptr) {
			// Not good!

			ErrorPopup::Open("The name of a directory can not contain any of the following characters: <, >, :, \", /, \\, |, ? and *.");
			return false;
		}

		// Todo(Nick): Handle case if directory already exists

		try {
			std::filesystem::create_directory(m_CurrentDirectory / buffer);
		}
		catch (std::exception e) {
			PINE_ASSERT(false, "Could not create directory: {0}.", e.what());
			ErrorPopup::Open(e.what());
			return false;
		}

		return true;
	}

}
