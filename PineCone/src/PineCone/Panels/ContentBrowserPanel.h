#pragma once

#include "Panel.h"
#include "PineCone\ImGui\UITextureLibrary.h"

#include <Pine.h>

#include <filesystem>
#include <memory>

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
		void RenderNodeView();

		void GotoPrevious();
		void GotoNext();
		void Goto(const std::filesystem::path& path, GotoAction action = GotoAction::None);

		bool MakeDir(const char* buffer);

	private:
		std::filesystem::path m_BaseDirectory = {};
		std::filesystem::path m_CurrentDirectory = {};

		// Todo(Nick) determine max size, and pop if exceeded
		std::vector<std::filesystem::path> m_PreviousDirectory;
		std::vector<std::filesystem::path> m_NextDirectory;

		UITextureLibrary* m_UITextureLibrary = nullptr;

		char m_ModalInputBuffer[255];
	};

}