#pragma once

#include <filesystem>

namespace Pine {

	class Texture2D {
	public:
		struct Specification {
			Specification() = default;
		};
	public:
		Texture2D(const Specification& spec, const std::filesystem::path& path);
		Texture2D(const Specification& spec, uint32_t width, uint32_t height, uint32_t channels, void* data, size_t size);
		virtual ~Texture2D();

		void Bind(uint32_t slot) const;

		uint32_t GetId() const { return m_TextureId; }

	private:
		void Load(void* data);

	private:
		uint32_t m_TextureId;

		Specification m_Spec;

		uint32_t m_Width, m_Height;
		uint32_t m_Channels;
	};

}