#include "pinepch.h"
#include "File.h"

namespace Pine {
	void FileStream::GetContents(const std::filesystem::path& filepath, std::string& result, bool binarySave)
	{
		PINE_PROFILE_FUNCTION();

		auto mode = binarySave ? std::ios::in | std::ios::binary : std::ios::in;

		std::ifstream file(filepath.string(), mode);

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
	
	void FileStream::PutContents(const std::filesystem::path& filepath, const char* data, size_t size)
	{
		PINE_PROFILE_FUNCTION();


		std::ofstream stream(filepath, std::ios::out | std::ios::trunc);

		if (stream.is_open()) {
			stream.write(data, size);

			stream.close();
		}
		else {
			PINE_ASSERT("Unable to open file '{0}' for writing", filepath);
		}
	}

}
