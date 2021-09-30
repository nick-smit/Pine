IncludeDir = {};
IncludeDir["glfw"]   = "pine/vendor/glfw/include";
IncludeDir["glad"]   = "pine/vendor/glad/include";
IncludeDir["glm"]    = "pine/vendor/glm";
IncludeDir["spdlog"] = "pine/vendor/spdlog/include";
IncludeDir["stb"]    = "pine/vendor/stb";

for lib, dir in pairs(IncludeDir) do
  IncludeDir[lib] = "%{wks.location}/" .. IncludeDir[lib]
end
