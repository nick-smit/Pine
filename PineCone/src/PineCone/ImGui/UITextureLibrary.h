#pragma once

#include <Pine.h>

#include <unordered_map>
#include <memory>
#include <utility>

namespace Pine {

	enum class UITexture
	{
		MissingTexture,

		FolderFill,
		FileFill,

		ArrowUpFill,
		ArrowLeftFill,
		ArrowDownFill,
		ArrowRightFill,
		
		CursorFill,
		TranslateFill,
		RotateFill,
		ScaleFill,

		VerticalDots,
	};

	enum class UITextureSize { 
		_48, 
		_96, 
		_240,

		Small = UITextureSize::_48,
		Medium = UITextureSize::_96,
		Large = UITextureSize::_240,
	};

	class UITextureLibrary
	{
	public:
		UITextureLibrary();
		virtual ~UITextureLibrary();

		std::shared_ptr<Texture2D> GetTexture(UITexture texture, UITextureSize size = UITextureSize::Small);
		uint32_t GetTextureID(UITexture texture, UITextureSize size = UITextureSize::Small);

		static UITextureLibrary* Get() { return s_UITextureLibrary; };
	
	private:
		void Load(UITexture texture, UITextureSize size);

	private:
		static UITextureLibrary* s_UITextureLibrary;

		std::unordered_map<UITexture, std::unordered_map<UITextureSize, std::shared_ptr<Texture2D>>> m_TextureMap;
	};

}