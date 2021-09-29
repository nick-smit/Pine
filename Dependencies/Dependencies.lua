IncludeDir = {};
IncludeDir["spdlog"] = "pine/vendor/spdlog/include";

for lib, dir in pairs(IncludeDir) do
  IncludeDir[lib] = "%{wks.location}/" .. IncludeDir[lib]
end
