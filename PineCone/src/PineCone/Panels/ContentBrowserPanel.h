#pragma once

#include "Panel.h"
#include "PineCone\ImGui\UITextureLibrary.h"

#include <Pine.h>

#include <filesystem>
#include <memory>

namespace Pine {

	class ContentBrowserPanel : public Panel
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender(Timestep ts) override;

	public:
		static std::string GetName() { return "ContentBrowserPanel"; };

	private:
		enum class GotoAction { None, Previous, Next };
		void GotoPrevious();
		void GotoNext();
		void Goto(const std::filesystem::path& path, GotoAction action = GotoAction::None);

	private:
		std::filesystem::path m_BaseDirectory = {};
		std::filesystem::path m_CurrentDirectory = {};

		// Todo(Nick) determine max size, and pop if exceeded
		std::vector<std::filesystem::path> m_PreviousDirectory;
		std::vector<std::filesystem::path> m_NextDirectory;

		UITextureLibrary* m_UITextureLibrary = nullptr;
	};

}