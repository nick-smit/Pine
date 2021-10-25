#pragma once

#include "Panel.h"
#include "PineCone\ImGui\UITextureLibrary.h"

#include <Pine.h>

#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

namespace Pine {

	class ContentBrowserPanel : public Panel
	{
	private:
		enum class GotoAction { None, Previous, Next };
	
	public:
		ContentBrowserPanel();

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "ContentBrowserPanel"; };

	private:
		void RenderTreeView();
		void RenderNodeView();
		
		void RenderDirectoryTree(const std::filesystem::path& path);

		void GotoPrevious();
		void GotoNext();
		void Goto(const std::filesystem::path& path, GotoAction action = GotoAction::None);

		bool MakeDir(const char* buffer) const;
		bool Move(const std::filesystem::path& src, const std::filesystem::path& dst);

		void DirectoryDropTarget(const std::filesystem::path& dst);
		void FileDropTarget(const std::filesystem::path& dst);

	private:
		std::vector<std::function<void()>> m_EventListeners;

		std::filesystem::path m_BaseDirectory = {};
		std::filesystem::path m_CurrentDirectory = {};

		// Todo(Nick) determine max size, and pop if exceeded
		std::vector<std::filesystem::path> m_PreviousDirectory;
		std::vector<std::filesystem::path> m_NextDirectory;

		UITextureLibrary* m_UITextureLibrary = nullptr;

		char m_ModalInputBuffer[255];

		bool m_ContentBrowserPanelFocused = false;
		bool m_ContentBrowserPanelHovered = false;
	};

}