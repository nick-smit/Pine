#include "pcpch.h"
#include "UITextureLibrary.h"

namespace Pine {

	UITextureLibrary* UITextureLibrary::s_UITextureLibrary = nullptr;

	UITextureLibrary::UITextureLibrary()
	{
		PINE_PROFILE_FUNCTION();

		s_UITextureLibrary = this;

		// Load textures into map
		UITextureSize textureSizes[] = { UITextureSize::_48, UITextureSize::_96, UITextureSize::_240 };
		
		for (const UITextureSize& size : textureSizes) {
			m_TextureMap[UITexture::FolderFill][size] = nullptr;
			m_TextureMap[UITexture::FileFill][size] = nullptr;
			m_TextureMap[UITexture::ArrowUpFill][size] = nullptr;
			m_TextureMap[UITexture::ArrowLeftFill][size] = nullptr;
			m_TextureMap[UITexture::ArrowDownFill][size] = nullptr;
			m_TextureMap[UITexture::ArrowRightFill][size] = nullptr;
			m_TextureMap[UITexture::CursorFill][size] = nullptr;
			m_TextureMap[UITexture::TranslateFill][size] = nullptr;
			m_TextureMap[UITexture::RotateFill][size] = nullptr;
			m_TextureMap[UITexture::ScaleFill][size] = nullptr;
		}
	}

	UITextureLibrary::~UITextureLibrary()
	{
		s_UITextureLibrary = nullptr;
	}

	std::shared_ptr<Texture2D> UITextureLibrary::GetTexture(UITexture texture, UITextureSize size)
	{
		PINE_PROFILE_FUNCTION();

		if (!m_TextureMap[texture][size]) {
			Load(texture, size);
		}

		return m_TextureMap[texture][size];
	}

	uint32_t UITextureLibrary::GetTextureID(UITexture texture, UITextureSize size)
	{
		return GetTexture(texture, size)->GetId();
	}

	void UITextureLibrary::Load(UITexture texture, UITextureSize size)
	{
		PINE_PROFILE_FUNCTION();

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
			case UITexture::CursorFill: {
				icon = "cursor-fill";
				break;
			}
			case UITexture::TranslateFill: {
				icon = "drag-move-2-line";
				break;
			}
			case UITexture::RotateFill: {
				icon = "clockwise-2-fill";
				break;
			}
			case UITexture::ScaleFill: {
				icon = "shape-fill";
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

		PINE_LOG_CORE_DEBUG("Loading UI Texture: '{0}'", iconPath);

		m_TextureMap[texture][size] = std::make_shared<Texture2D>(iconPath);
	}

}
