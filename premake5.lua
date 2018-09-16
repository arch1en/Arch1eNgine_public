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
files({"Source/**.h", "Source/**.cpp", "Source/**.inl"})
-- use removefiles function to remove any unnescessary files.

-- Including directories...
includedirs(WorkspaceDirectory.. "/Source") 
for i in pairs(Dependencies) do
    local Size = #Dependencies[i]["IncludeDirs"]
    for j=1,Size do
        includedirs(AdaptDirSlashes(GetDependencySourceDir(Dependencies[i]["Name"]).. "/" ..Dependencies[i]["IncludeDirs"][j]))
    end
end

-- Linking libraries...
for _,v0 in pairs(Dependencies) do

    local PropertyGroups = v0["PropertyGroups"]
    if PropertyGroups ~= nil and #PropertyGroups ~= 0 then
        for _,v1 in ipairs(PropertyGroups) do
            filter("configurations:" ..GetPropertyGroupConfigurationName(v1["Name"]))
            if v1["LinkFileNames"] ~= nil and #v1["LinkFileNames"] ~= 0 then
                for _,v2 in ipairs(v1["LinkFileNames"]) do
                    if v2 ~= "" then
                        links(v2)
                    end
                end
            else
                print("Linking libraries : " ..v1["Name"].. " property group in " ..v0["Name"].. " dependency has no files to link")
            end

        end

    else
        print("Linking libraries : "..v0["Name"].. " dependency has no property groups")
    end

end

filter {}
-- Finding libraries...
for i in pairs(Dependencies) do
    if Dependencies[i]["RequiresBuilding"] == true then
    local DependencyName = Dependencies[i]["Name"]
        for _,c in pairs(Configurations) do
            for _,p in pairs(Platforms) do
                local LibraryDir = AdaptDirSlashes(GetDependencyLibrariesDir(DependencyName).. "/" ..c["Name"].. "/" ..p["Name"])
                if os.isdir(LibraryDir) then
                    libdirs(LibraryDir)
                end
            end
        end
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
systemversion(os.winSdkVersion() .. ".0")
pchheader "stdafx.h"
pchsource "stdafx.cpp"
--flags { "/Ycstdafx.cpp" }
