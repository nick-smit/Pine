#pragma once

#include <Pine.h>

#include <unordered_map>
#include <memory>

namespace Pine {

	enum class UITexture
	{
		MissingTexture,

		Folder,
		File,

		ArrowUp,
		ArrowLeft,
		ArrowDown,
		ArrowRight,
	};

	class UITextureLibrary
	{
	public:
		UITextureLibrary();
		virtual ~UITextureLibrary();

		std::shared_ptr<Texture2D> GetTexture(UITexture texture);
		uint32_t GetTextureID(UITexture texture);

		static UITextureLibrary* Get() { return s_UITextureLibrary; };
	
	private:
		static UITextureLibrary* s_UITextureLibrary;

		std::unordered_map<UITexture, std::shared_ptr<Texture2D>> m_TextureMap;
	};

}