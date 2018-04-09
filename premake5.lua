
-- List of dependencies that doesnt require building :
-- Dear ImGui
-- GLM
--

require("premake5_functions")
require("premake5_actions")
require("premake5_options")
Rebuilds = {}

-- CONFIGURATIONS --
RebuildAll = true

Rebuilds["ASSIMP"] = true
--------------------

BuildsDir = "Builds"
DependenciesDir = "Dependencies"

WorkspaceDirectory = io.popen("cd"):read('*l'):gsub("\\","/") -- ugly hack, because premake tokens doesnt work -.-

DependencyNames = {}

DependencyNames["GLM"] = "GLM"
DependencyNames["ASSIMP"] = "ASSIMP"
DependencyNames["GLEW"] = "GLEW"
DependencyNames["SDL2"] = "SDL2"
DependencyNames["DevIL"] = "DevIL"
DependencyNames["FreeType"] = "FreeType"

DependencyLinkFileNames = {}

DependencyLinkFileNames["ASSIMP"] = "assimp-vc140-mt"
DependencyLinkFileNames["GLEW"]  = "glew32"
DependencyLinkFileNames["SDL2"] = "SDL2"
DependencyLinkFileNames["SDL2main"] = "SDL2main"
DependencyLinkFileNames["DevIL"] = "DevIL"
DependencyLinkFileNames["FreeType"] = ""

DependencyDirs = {}

DependencyDirs["ASSIMP"] = WorkspaceDirectory .. "/" .. DependenciesDir .. "/" .. DependencyNames["ASSIMP"]
DependencyDirs["GLM"] = WorkspaceDirectory .. "/" .. DependenciesDir .. "/" .. DependencyNames["GLM"]
DependencyDirs["GLEW"] = WorkspaceDirectory .. "/" .. DependenciesDir .. "/" .. DependencyNames["GLEW"]
DependencyDirs["SDL2"] = WorkspaceDirectory .. "/" .. DependenciesDir .. "/" .. DependencyNames["SDL2"]
DependencyDirs["DevIL"] = WorkspaceDirectory .. "/" .. DependenciesDir .. "/" .. DependencyNames["DevIL"]
DependencyDirs["FreeType"] = WorkspaceDirectory .. "/" .. DependenciesDir .. "/" .. DependencyNames["FreeType"]

BuildDirs = {}

BuildDirs["ASSIMP"] = WorkspaceDirectory .. "/" .. BuildsDir .. "/" .. DependencyNames["ASSIMP"]
BuildDirs["GLM"] = WorkspaceDirectory .. "/" .. BuildsDir .. "/" .. DependencyNames["GLM"]
BuildDirs["GLEW"] = WorkspaceDirectory .. "/" .. BuildsDir .. "/" .. DependencyNames["GLEW"]
BuildDirs["SDL2"] = WorkspaceDirectory .. "/" .. BuildsDir .. "/" .. DependencyNames["SDL2"]
BuildDirs["DevIL"] = WorkspaceDirectory .. "/" .. BuildsDir .. "/" .. DependencyNames["DevIL"]
BuildDirs["FreeType"] = WorkspaceDirectory .. "/" .. BuildsDir .. "/" .. DependencyNames["FreeType"]

DependencyLinkDirs = {}

DependencyLinkDirs["GLM"] = ""
DependencyLinkDirs["ASSIMP"] = ""
DependencyLinkDirs["GLEW"] = ""
DependencyLinkDirs["SDL2"] = ""
DependencyLinkDirs["DevIL"] = ""
DependencyLinkDirs["FreeType"] = ""

DependencyIncludeDirs = {}

DependencyIncludeDirs["GLM"] = ""
DependencyIncludeDirs["ASSIMP"] = ""
DependencyIncludeDirs["GLEW"] = ""
DependencyIncludeDirs["SDL2"] = ""
DependencyIncludeDirs["DevIL"] = ""
DependencyIncludeDirs["FreeType"] = ""

if RebuildAll == true then
	printf("Rebuilding solution...")
	os.rmdir(WorkspaceDirectory .. "/" .. BuildsDir)
	os.mkdir(WorkspaceDirectory .. "/" .. BuildsDir)
end

workspace("Arch1eNgine")
	configurations({ "Debug", "Release" })
	platforms({"Win32", "Win64", "Linux"})
	location("Builds")

-- Prepare dependencies first, so that the engine target will have all required stuff ready.

	-- GLM --
	CurrentProjectConfigName = "GLM"
		-- Including directories...
		DependencyIncludeDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName]

	-- ASSIMP --
	CurrentProjectConfigName = "ASSIMP"
	--project(DependencyNames[CurrentProjectConfigName])
		--kind("SharedLib")
		--targetdir("Binaries/")
		--location(BuildDirs[CurrentProjectConfigName])

		if Rebuilds["ASSIMP"] == true then
			os.rmdir(BuildDirs[CurrentProjectConfigName])
			os.mkdir(BuildDirs[CurrentProjectConfigName])


			--io.popen("cmake --build " .. BuildDirs[CurrentProjectConfigName])
		end

		--os.execute("cmake -E remove_directory " .. BuildDirs[CurrentProjectConfigName])
		--os.execute("cmake -E make_directory " .. BuildDirs[CurrentProjectConfigName])

		-- Including directories...
		DependencyIncludeDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/include"
		-- Linking directory...
		if os.is64bit() then
			DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/lib/Release" -- This is the same for 32 and 64 bit machine, change that later.
		else
			DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/lib/Release" -- This is the same for 32 and 64 bit machine, change that later.
		end

	-- GLEW --
	CurrentProjectConfigName = "GLEW"
	project(DependencyNames[CurrentProjectConfigName])
		kind("SharedLib")
		targetdir("Binaries/")
		location(BuildDirs[CurrentProjectConfigName])
		-- Including directories...
		DependencyIncludeDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/include"
		-- Linking directory...
		DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/lib/Release/"
		if os.is64bit() then
			DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x64"
		else
			DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/Win32"
		end

	-- SDL2 --
	CurrentProjectConfigName = "SDL2"
	project(DependencyNames[CurrentProjectConfigName])
		kind("SharedLib")
		targetdir("Binaries/")
		location(BuildDirs[CurrentProjectConfigName])
		-- Including directories...
		DependencyIncludeDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/include"
		-- Linking directory...
		DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/lib/"
		if os.is64bit() then
			DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x64"
		else
			DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x86"
		end

	-- DevIL --
	CurrentProjectConfigName = "DevIL"
	project(DependencyNames[CurrentProjectConfigName])
		kind("SharedLib")
		targetdir("Binaries/")
		location(BuildDirs[CurrentProjectConfigName])
		-- Including directories...
		DependencyIncludeDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/WIN/include"
		-- Linking directory...
		DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/WIN/lib"
		if os.is64bit() then
			DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x64/Release"
		else
			DependencyLinkDirs[CurrentProjectConfigName] = DependencyLinkDirs[CurrentProjectConfigName] .. "/x86/Release"
		end

project("Engine")
	kind("ConsoleApp")
	language("C++")
	targetdir("Binaries/%{cfg.buildcfg}")
	location("Builds/%{prj.name}")

	-- Adding source files to the project...
	files({"Source/**.h", "Source/**.cpp"})
	-- use removefiles function to remove any unnescessary files.

	-- Including directories...
	for i in pairs(DependencyIncludeDirs) do
		includedirs(DependencyIncludeDirs[i])
	end

	-- Linking libraries...
	for i in pairs(DependencyLinkFileNames) do
		links(DependencyLinkFileNames[i])
	end

	-- Finding libraries...
	for i in pairs(DependencyLinkDirs) do
		libdirs(DependencyLinkDirs[i])
	end

configuration("windows")
	postbuildcommands {
		"cmake --build " .. BuildDirs[CurrentProjectConfigName]
	}

	filter("system:windows")
		links({"OpenGL32"})

	filter("system:not windows")
		links({"GL"})

	filter("configurations:Debug")
		defines({"DEBUG"})
		symbols("On")

	filter("configurations:Release")
		optimize("On")
