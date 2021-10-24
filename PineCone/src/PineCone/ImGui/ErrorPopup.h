#pragma once

namespace Pine {

	class ErrorPopup
	{
	public:
		static void Render();

		static void Open(const std::string& reason);

	private:
		static bool s_IsOpen;
		static std::string s_Reason;
	};

}