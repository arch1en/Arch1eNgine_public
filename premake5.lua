require("premake5_config")
require("premake5_functions")
require("premake5_actions")
require("premake5_options")



Rebuilds = {}

workspace("Arch1eNgine")
	configurations({ "Debug", "Release" })
	platforms({"Win32", "Win64", "Linux"})
	location("Builds")


	-- GLEW --
	--CurrentProjectConfigName = "GLEW"
	--project(DependencyNames[CurrentProjectConfigName])
	--	kind("SharedLib")
	--	targetdir("Binaries/")
	--	location(BuildDirs[CurrentProjectConfigName])
		-- Including directories...
	--	DependencyIncludeDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/include"
		-- Linking directory...
	--	DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/lib/Release/"
	--	if os.is64bit() then
	--		DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x64"
	--	else
	--		DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/Win32"
	--	end

	-- SDL2 --
	--CurrentProjectConfigName = "SDL2"
	--project(DependencyNames[CurrentProjectConfigName])
	--	kind("SharedLib")
	--	targetdir("Binaries/")
	--	location(BuildDirs[CurrentProjectConfigName])
		-- Including directories...
	--	DependencyIncludeDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/include"
		-- Linking directory...
	--	DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/lib/"
	--	if os.is64bit() then
	--		DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x64"
	--	else
	--		DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x86"
	--	end

	-- DevIL --
	--CurrentProjectConfigName = "DevIL"
	--project(DependencyNames[CurrentProjectConfigName])
	--	kind("SharedLib")
	--	targetdir("Binaries/")
	--	location(BuildDirs[CurrentProjectConfigName])
		-- Including directories...
	--	DependencyIncludeDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/WIN/include"
		-- Linking directory...
--		DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/WIN/lib"
--		if os.is64bit() then
--			DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x64/Release"
--		else
--			DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x86/Release"
--		end

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
	for i in pairs(Dependencies) do
		local Size = #Dependencies[i]["LinkFileNames"]
		for j=1,Size do
			local FileName = Dependencies[i]["LinkFileNames"][j]
			if FileName ~= "" then
				links(FileName)
			end
		end
	end

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

	filter("configurations:Debug")
		defines({"DEBUG"})
		symbols("On")

	filter("configurations:Release")
		optimize("On")

	filter("action:vs*")
		pchheader "Source/stdafx.h"
		pchsource "Source/stdafx.cpp"
