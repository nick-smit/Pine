#include "pinepch.h"
#include "File.h"

namespace Pine {
	void FileStream::GetContents(const std::string& filepath, std::string& result, bool binarySave)
	{
		PINE_PROFILE_FUNCTION();

		auto mode = binarySave ? std::ios::in | std::ios::binary : std::ios::in;

		std::ifstream file(filepath, mode);

		if (file.is_open()) {
			file.seekg(0, std::ios::end);
			size_t size = file.tellg();
			if (size != -1) {
				result.resize(size);
				file.seekg(0, std::ios::beg);
				file.read(&result[0], size);
			}
			else {
				PINE_ASSERT("Could not read from file '{0}'", filepath);
			}
		}
		else {
			PINE_ASSERT("Unable to open file '{0}'", filepath);
		}
	}
}
