#include "pcpch.h"
#include "UITextureLibrary.h"

namespace Pine {

	UITextureLibrary* UITextureLibrary::s_UITextureLibrary = nullptr;

	UITextureLibrary::UITextureLibrary()
	{
		s_UITextureLibrary = this;

		// Load textures into map
		UITextureSize[] textureSizes = { UITextureSize::_48, UITextureSize::_96, UITextureSize::_240 };
		
		for (const UITextureSize& size : textureSizes) {
			m_LoadedTextures[UITexture::FolderFill][size] = nullptr;
			m_LoadedTextures[UITexture::FileFill][size] = nullptr;
			m_LoadedTextures[UITexture::ArrowUpFill][size] = nullptr;
			m_LoadedTextures[UITexture::ArrowLeftFill][size] = nullptr;
			m_LoadedTextures[UITexture::ArrowDownFill][size] = nullptr;
			m_LoadedTextures[UITexture::ArrowRightFill][size] = nullptr;
		}
	}

	UITextureLibrary::~UITextureLibrary()
	{
		s_UITextureLibrary = nullptr;
	}

	std::shared_ptr<Texture2D> UITextureLibrary::GetTexture(UITexture texture, UITextureSize size)
	{
		if (!m_TextureMap[texture][size]) {
			Load(texture, size);
		}

		return m_TextureMap[texture][size];
	}

	uint32_t UITextureLibrary::GetTextureID(UITexture texture)
	{
		return GetTexture(texture)->GetId();
	}

	void Load(UITexture texture, UITextureSize size)
	{
		// TODO(Nick) We might want to optimize this, but for the time being this is fine.

		auto iconBasePath = std::filesystem::current_path() / "editor_assets/icon";

		std::string icon;
		switch (texture) {
			case UITexture::FolderFill: {
				icon = "folder-5-fill";
				break;
			}
			case UITexture::FileFill: {
				icon = "file-fill";
				break;
			}
			case UITexture::ArrowUpFill: {
				icon = "arrow-up-circle-fill";
				break;
			}
			case UITexture::ArrowRightFill: {
				icon = "arrow-right-circle-fill";
				break;
			}
			case UITexture::ArrowDownFill: {
				icon = "arrow-down-circle-fill";
				break;
			}
			case UITexture::ArrowLeftFill: {
				icon = "arrow-left-circle-fill";
				break;
			}
		}

		switch (size) {
			case UITextureSize::_48: {
				icon.append("-48.png");
				break;
			}
			case UITextureSize::_96: {
				icon.append("-96.png");
				break;
			}
			case UITextureSize::_240: {
				icon.append("-240.png");
				break;
			}
			default: PINE_ASSERT(false, "Unknown icon size");
		}

		auto iconPath = iconBasePath / icon;
		PINE_ASSERT(std::filesystem::exists(iconPath), "Icon could not be found!");

		m_TextureMap[texture][size] = std::make_shared<Texture2D>(iconPath);
	}

}
