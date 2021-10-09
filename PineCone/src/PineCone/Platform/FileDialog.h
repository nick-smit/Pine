#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace Pine {

	class FileDialog {
	public:
		struct Filter {
			std::wstring HumanReadableName;
			std::wstring AcceptedTypes;

			Filter() = default;
		};

	public:
		static std::filesystem::path OpenFile(const std::vector<Filter>& filter);
		static std::filesystem::path SaveFile(const std::vector<Filter>& filter);
	};

}
