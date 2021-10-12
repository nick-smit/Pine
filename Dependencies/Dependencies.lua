IncludeDir = {};
IncludeDir["entt"]   = "Pine/vendor/entt/single_include";
IncludeDir["glfw"]   = "Pine/vendor/glfw/include";
IncludeDir["glad"]   = "Pine/vendor/glad/include";
IncludeDir["glm"]    = "Pine/vendor/glm";
IncludeDir["ImGui"]  = "PineCone/vendor/imgui"
IncludeDir["spdlog"] = "Pine/vendor/spdlog/include";
IncludeDir["stb"]    = "Pine/vendor/stb";
IncludeDir["yaml_cpp"]    = "Pine/vendor/yaml-cpp/include";

for lib, dir in pairs(IncludeDir) do
  IncludeDir[lib] = "%{wks.location}/" .. IncludeDir[lib]
end
