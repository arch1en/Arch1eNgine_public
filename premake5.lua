require("premake5_config")
require("premake5_functions")
require("premake5_actions")
require("premake5_options")

workspace("Arch1eNgine")
local ConfigurationNames = {}
local PlatformNames = {}
for i,c in pairs(Configurations) do
    ConfigurationNames[i] = c.Name
    for j,p in pairs(Platforms) do
        PlatformNames[j] = p.Name
    end
end

filter{}

configurations(ConfigurationNames)
platforms(PlatformNames)
location("Application")

project("Application")
kind("ConsoleApp")
language("C++")
targetdir(GetBinariesDir().."/%{cfg.buildcfg}/%{cfg.platform}")
location("Application/%{prj.name}")

-- Adding source files to the project...
files({"%{prj.name}/Source/**.h", "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.inl"})
-- use removefiles function to remove any unnescessary files.

-- Including directories...
includedirs(WorkspaceDirectory.. "/Source") 
for _,v in pairs(GetListOfDependencyNames()) do

	local DependencyProperties = GetDependencyPropertiesByName(v)
	if DependencyProperties == nil then
		print("Including "..v.. " directories : Action failed. Properties not found.")
		return
	end
	local Size = #DependencyProperties.IncludeDirs
    for j=1,Size do
        includedirs(AdaptDirSlashes(GetDependencyDir(DependencyProperties.Name).. "/" ..DependencyProperties.IncludeDirs[j]))
    end
end

-- Linking libraries...
for _,v0 in pairs(GetListOfDependencyNames()) do
    LinkForeignDependency(GetDependencyPropertiesByName(v0))
end

filter {}
-- Finding libraries...
for _,v in pairs(GetListOfDependencyNames()) do
    AddForeignDependencyLibraryDirs(GetDependencyPropertiesByName(v))
end

configuration("windows")

filter("system:windows")
links({"OpenGL32"})

filter("system:not windows")
links({"GL"})

filter {}

-- Adding configuration defines
for i,v in pairs(Configurations) do
    filter("configurations:" .. v.Name)
    if #v.Defines ~= 0 then
        local DefinesNames = {}
        for j,w in pairs(v.Defines) do
            DefinesNames[j] = w
        end
        defines(DefinesNames)
    end

    symbols(v.Symbols)
    optimize(v.Optimize)

end

filter{}

for _,v in pairs(Platforms) do
    if #v.Defines ~= 0 then
        filter{"platforms:"..v.Name}
        local NewDefines = {}
        for i,w in pairs(v.Defines) do
            NewDefines[i] = w
        end
        defines(NewDefines)
    end
end

filter{}

filter{"action:vs*"}
systemversion(os.winSdkVersion() .. ".0")
pchheader "stdafx.h"
pchsource "stdafx.cpp"

filter{}

-- Add module projects

for _,v in pairs(GetListOfModuleNames()) do
	SetupModule(v)
end

for _,v in pairs(GetListOfDependencyNames()) do

	local DependencyProperties = GetDependencyPropertiesByName(v)
	if DependencyProperties == nil then
		print("PostPremakeCommand "..v0.. " : Action failed. Properties not found.")
		return
	end
	
    if v["PostPremakeCommand"] ~= nil then
        v["PostPremakeCommand"]()
    end
end
