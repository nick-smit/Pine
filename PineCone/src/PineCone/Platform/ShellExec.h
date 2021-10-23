#pragma once

#include <filesystem>

namespace Pine {

	class ShellExec
	{
	public:
		static void Open(const std::filesystem::path& path);
	};

}