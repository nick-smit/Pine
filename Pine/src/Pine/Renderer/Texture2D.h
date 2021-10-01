#pragma once

#include <string>

namespace Pine {

	class Texture2D {
	public:
		struct Specification {
			Specification() = default;
		};
	public:
		Texture2D(const Specification& spec, const std::string& path);
		Texture2D(const Specification& spec, uint32_t width, uint32_t height, uint32_t channels, unsigned char* data);
		virtual ~Texture2D();

		void Bind(uint32_t slot) const;

	private:
		void Load(unsigned char* data);

	private:
		uint32_t m_TextureId;

		Specification m_Spec;

		uint32_t m_Width, m_Height;
		uint32_t m_Channels;
	};

}