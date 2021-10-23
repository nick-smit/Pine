#include "pcpch.h"
#include "ShellExec.h"

namespace Pine {

	#ifdef WIN32
	void ShellExec::Open(const std::filesystem::path& path)
	{
		ShellExecuteA(GetDesktopWindow(), "open", NULL, NULL, path.string().c_str(), SW_SHOW);
	}
	#endif
}
