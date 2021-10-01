#pragma once

namespace Pine {

	class Renderer {
	public:
		enum class Face { None, FrontLeft, FrontRight, BackLeft, BackRight, Front, Back, Left, Right, FrontAndBack };
		enum class PolygonMode { Lines, Fill };

	public:
		static void Init();
		static void Terminate();
	};

}
