require("premake5_config")
require("premake5_functions")
require("premake5_actions")
require("premake5_options")


Rebuilds = {}

workspace("Arch1eNgine")
local ConfigurationNames = {}
for i,v in pairs(Configurations) do
    ConfigurationNames[i] = v["Name"]
end
configurations(ConfigurationNames)
platforms({"Win32", "Win64", "Linux"})
location("Builds")

project("Engine")
kind("ConsoleApp")
language("C++")
targetdir("Binaries/%{cfg.buildcfg}")
location("Builds/%{prj.name}")

-- Adding source files to the project...
files({"Source/**.h", "Source/**.cpp"})
-- use removefiles function to remove any unnescessary files.

-- Including directories...
for i in pairs(Dependencies) do
    local Size = #Dependencies[i]["IncludeDirs"]
    for j=1,Size do
        includedirs(DependencyDirs[Dependencies[i]["Name"]].. "/" ..Dependencies[i]["IncludeDirs"][j])
    end
end

-- Linking libraries...
for _,v0 in pairs(Dependencies) do
    for _,v1 in ipairs(v0["ConfigurationProperties"]) do
        for i,v2 in ipairs(v1) do
            filter("configurations:" ..Configurations[i]["Name"])
            if v2["LinkFileNames"] ~= nil and #v2["LinkFileNames"] ~= 0 then
                for _,v3 in ipairs(v2["LinkFileNames"]) do
                    if v3 ~= "" then
                        links(v3)
                    end
                end
            end
        end
--      local Size = #v1["LinkFileNames"]
--      for j=1,Size do
--          local FileName = Dependencies[i]["LinkFileNames"][j]
--          if FileName ~= "" then
--              links(FileName)
--          end
--      end
    end
end

filter {}
-- Finding libraries...
for i in pairs(Dependencies) do
    if Dependencies[i]["RequiresBuilding"] == true then
        libdirs(WorkspaceDirectory.. "/" ..BuildsDir.. "/Libraries/" ..Dependencies[i]["Name"])
    end
end

configuration("windows")
--postbuildcommands {
--	"cmake --build " .. BuildDirs[CurrentProjectConfigName]
--}

filter("system:windows")
links({"OpenGL32"})

filter("system:not windows")
links({"GL"})

for i,v in pairs(Configurations) do
    filter("configurations:" .. v["Name"])
    if #v["Defines"] ~= 0 then
        local DefinesNames = {}
        for j,w in pairs(v["Defines"]) do
            DefinesNames[j] = w
        end
        defines(DefinesNames)
    end

    symbols(v["Symbols"])
    optimize(v["Optimize"])

end

filter("action:vs*")
pchheader "Source/stdafx.h"
pchsource "Source/stdafx.cpp"
