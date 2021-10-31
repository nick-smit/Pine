#include "pcpch.h"
#include "ContentBrowserPanel.h"

#include "PineCone\Core\Event.h"
#include "PineCone\ImGui\UI.h"
#include "PineCone\ImGui\ErrorPopup.h"
#include "PineCone\Platform\FileDialog.h"
#include "PineCone\Platform\ShellExec.h"

#include <imgui.h>
#include <imgui_internal.h>

/// <summary>
/// Wanted features
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

		UI::BeginWindowWithToolbar("Content browser");

		m_ContentBrowserPanelFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy);
		m_ContentBrowserPanelHovered = ImGui::IsWindowHovered(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy);

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
			UI::EndWindowWithToolbar();
			return;
		}
		
		RenderTreeView();
		
		ImGui::SameLine();

		RenderNodeView();

		UI::EndWindowWithToolbar();
	}

	void ContentBrowserPanel::RenderTreeView()
	{
		PINE_PROFILE_FUNCTION();

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

		const float actionButtonSize = UI::GetToolbarButtonHeight();
		float actionBarPadding = (float)style.WindowPadding.y;

		UI::BeginWindowWithToolbar_Toolbar();

		{
			if (UI::Button(UITexture::ArrowLeftFill, actionButtonSize, m_PreviousDirectory.empty() ? UI::Status::Disabled : UI::Status::None)) {
				GotoPrevious();
			}

			if (UI::Button(UITexture::ArrowRightFill, actionButtonSize, m_NextDirectory.empty() ? UI::Status::Disabled : UI::Status::None)) {
				GotoNext();
			}

			if (UI::Button(UITexture::ArrowUpFill, actionButtonSize, m_CurrentDirectory == m_BaseDirectory ? UI::Status::Disabled : UI::Status::None)) {
				Goto(m_CurrentDirectory.parent_path());
			}

			UI::ToolbarSeparator();

			auto relativePath = std::filesystem::relative(m_CurrentDirectory, m_BaseDirectory);
			
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

			if (UI::Button(m_BaseDirectory.filename().string(), { 0.0f, actionButtonSize })) {
				Goto(m_BaseDirectory);
			}
			if (ImGui::BeginDragDropTarget()) {
				DirectoryDropTarget(m_BaseDirectory);
				FileDropTarget(m_BaseDirectory);

				ImGui::EndDragDropTarget();
			}
			
			if (m_CurrentDirectory != m_BaseDirectory) {
				std::filesystem::path clickedPath = m_BaseDirectory;
				for (const auto& dirname : relativePath) {
					ImGui::Text("/");

					clickedPath /= dirname;

					if (UI::Button(dirname.string(), { 0.0f, actionButtonSize })) {
						Goto(clickedPath);
					}
					
					if (ImGui::BeginDragDropTarget()) {
						DirectoryDropTarget(clickedPath);
						FileDropTarget(clickedPath);

						ImGui::EndDragDropTarget();
					}
				}
			}

			ImGui::PopStyleVar();
		} 
		UI::EndWindowWithToolbar_Toolbar();
		UI::BeginWindowWithToolbar_Content();

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
		constexpr UITextureSize texSize = UITextureSize::_96;
		ImVec4 iconColor = UI::GetColor("Icon_Default");

		for (const auto& node : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const std::string& fn = node.path().filename().string();
			
			ImGui::PushID(fn.c_str());

			if (node.is_directory()) {
				ImGui::BeginGroup();
				ImGui::Image((void*)m_UITextureLibrary->GetTextureID(UITexture::FolderFill, texSize), nodeSize, ImVec2(0, 0), ImVec2(1, 1), iconColor);
				UI::TextCentered(node.path().stem().string(), (float)nodeSize.x);
				ImGui::EndGroup();

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					dragDropPayload = node.path();
					ImGui::SetDragDropPayload("directory", &dragDropPayload, sizeof(dragDropPayload));

					ImGui::Text(node.path().filename().string().c_str());

					ImGui::EndDragDropSource();
				}

				if (ImGui::BeginDragDropTarget()) {
					DirectoryDropTarget(node.path());
					FileDropTarget(node.path());

					ImGui::EndDragDropTarget();
				}
			}
			else {
				ImGui::BeginGroup();
				ImGui::Image((void*)m_UITextureLibrary->GetTextureID(UITexture::FileFill, texSize), nodeSize, ImVec2(0, 0), ImVec2(1, 1), iconColor);
				UI::TextCentered(node.path().filename().string(), (float)nodeSize.x);
				ImGui::EndGroup();

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					dragDropPayload = node.path();
					ImGui::SetDragDropPayload("file", &dragDropPayload, sizeof(dragDropPayload));

					ImGui::Text(node.path().filename().string().c_str());
					
					ImGui::EndDragDropSource();
				}
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				ExecuteMainAction(node);
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

		UI::EndWindowWithToolbar_Content();

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

			for (const auto& subDir : std::filesystem::directory_iterator(directory)) {
				if (subDir.is_directory()) {
					baseFlags &= ~ImGuiTreeNodeFlags_Leaf;
					break;
				}
			}

			bool open = ImGui::TreeNodeEx(directory.path().filename().string().c_str(), baseFlags);
			if (ImGui::IsItemClicked()) {
				Goto(directory);
			}

			if (ImGui::BeginDragDropSource()) {
				dragDropPayload = directory.path();
				ImGui::SetDragDropPayload("directory", &dragDropPayload, sizeof(dragDropPayload));

				ImGui::Text(directory.path().filename().string().c_str());

				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {
				DirectoryDropTarget(directory);
				FileDropTarget(directory);

				ImGui::EndDragDropTarget();
			}

			if (open) {
				RenderDirectoryTree(directory);

				ImGui::TreePop();
			}

		}
	}

	void ContentBrowserPanel::ExecuteMainAction(const std::filesystem::directory_entry& entry)
	{
		if (entry.is_directory()) {
			Goto(entry.path());
			return;
		}

		if (!entry.path().has_extension()) {
			return;
		}

		const auto& ext = entry.path().extension();
		if (ext == ".pinescene") {
			EventDispatcher<SceneOpenedEvent>::Dispatch({ entry.path() });
		}
		else {
			PINE_LOG_CORE_WARN("Unable to execute main action for {0}", entry.path().filename());
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

	bool ContentBrowserPanel::MakeDir(const char* buffer) const
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

	bool ContentBrowserPanel::Move(const std::filesystem::path& src, const std::filesystem::path& dst)
	{
		if (std::filesystem::exists(src / dst.filename())) {
			// Todo: Confirmation for overwriting directory
			ErrorPopup::Open(fmt::format("Cannot move file '{0}' because a file with the same name exists in '{1}'", src.filename(), dst));
			return false;
		}

		std::filesystem::rename(src, dst / src.filename());
		
		if (src == m_CurrentDirectory) {
			m_CurrentDirectory = dst / src.filename();
		}

		return true;
	}

	void ContentBrowserPanel::DirectoryDropTarget(const std::filesystem::path& dst)
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("directory")) {
			const std::filesystem::path src = *(const std::filesystem::path*)payload->Data;

			Move(src, dst);
		}
	}

	void ContentBrowserPanel::FileDropTarget(const std::filesystem::path& dst)
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file")) {
			const std::filesystem::path src = *(const std::filesystem::path*)payload->Data;

			Move(src, dst);
		}
	}

}
