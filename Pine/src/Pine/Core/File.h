#pragma once

#include <filesystem>
#include <string>

namespace Pine {

	class FileStream {
	public:
		static void GetContents(const std::filesystem::path& filepath, std::string& result, bool binarySave = true);

		static void PutContents(const std::filesystem::path& filepath, const char* data, size_t size);
	};

}
