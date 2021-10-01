IncludeDir = {};
IncludeDir["glfw"]   = "Pine/vendor/glfw/include";
IncludeDir["glad"]   = "Pine/vendor/glad/include";
IncludeDir["glm"]    = "Pine/vendor/glm";
IncludeDir["spdlog"] = "Pine/vendor/spdlog/include";
IncludeDir["stb"]    = "Pine/vendor/stb";

for lib, dir in pairs(IncludeDir) do
  IncludeDir[lib] = "%{wks.location}/" .. IncludeDir[lib]
end
