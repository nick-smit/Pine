include "Dependencies/Dependencies.lua"

workspace "Pine"
  architecture "x64"
	startproject "PineCone"
  
  configurations {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
targetDir = "%{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
objDir = "%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}"


group "Dependencies"
	include "Dependencies/glad"
	include "Dependencies/glm"
	include "Dependencies/glfw"
	include "Dependencies/ImGui"
	include "Dependencies/ImGuizmo"
	include "Dependencies/spdlog"
	include "Dependencies/yaml-cpp"

group ""
	include "Pine"
	include "PineCone"
	include "Sandbox"
