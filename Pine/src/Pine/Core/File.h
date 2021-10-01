#pragma once

#include <fstream>
#include <memory>
#include <string>

namespace Pine {

	class FileStream {
	public:
		static void GetContents(const std::string& filepath, std::string& result, bool binarySave = true);
	};

}
