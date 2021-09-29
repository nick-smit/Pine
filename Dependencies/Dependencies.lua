IncludeDir = {};
IncludeDir["spdlog"] = "Pine/vendor/spdlog/include";
IncludeDir["glfw"]   = "Pine/vendor/glfw/include";

for lib, dir in pairs(IncludeDir) do
  IncludeDir[lib] = "%{wks.location}/" .. IncludeDir[lib]
end
