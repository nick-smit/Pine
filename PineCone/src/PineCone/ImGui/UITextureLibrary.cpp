#include "pcpch.h"
#include "UITextureLibrary.h"

namespace Pine {

	UITextureLibrary* UITextureLibrary::s_UITextureLibrary = nullptr;

	UITextureLibrary::UITextureLibrary()
	{
		s_UITextureLibrary = this;

		auto iconPath = std::filesystem::current_path() / "editor_assets/icon";

		// Load textures into map
		m_TextureMap[UITexture::Folder] = std::make_shared<Texture2D>(iconPath / "folder.png");
		m_TextureMap[UITexture::File] = std::make_shared<Texture2D>(iconPath / "file.png");

		m_TextureMap[UITexture::ArrowUp] = std::make_shared<Texture2D>(iconPath / "arrow-up.png");
		m_TextureMap[UITexture::ArrowLeft] = std::make_shared<Texture2D>(iconPath / "arrow-left.png");
		m_TextureMap[UITexture::ArrowRight] = std::make_shared<Texture2D>(iconPath / "arrow-right.png");
		m_TextureMap[UITexture::ArrowDown] = std::make_shared<Texture2D>(iconPath / "arrow-down.png");
	}

	UITextureLibrary::~UITextureLibrary()
	{
		s_UITextureLibrary = nullptr;
	}

	std::shared_ptr<Texture2D> UITextureLibrary::GetTexture(UITexture texture)
	{
		PINE_ASSERT(m_TextureMap.find(texture) != m_TextureMap.end(), "Texture not found!");

		return m_TextureMap[texture];
	}

	uint32_t UITextureLibrary::GetTextureID(UITexture texture)
	{
		return GetTexture(texture)->GetId();
	}

}
