#pragma once

#include <filesystem>
#include <string>

namespace Pine {

	class FileStream {
	public:
		static void GetContents(const std::filesystem::path& filepath, std::string& result, bool binarySave = true);
	};

}
