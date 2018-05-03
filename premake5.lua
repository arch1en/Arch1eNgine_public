
-- List of dependencies that doesnt require building :
-- Dear ImGui
-- GLM
--

BuildsDir = "Builds"
DependenciesDir = "Dependencies"
WorkspaceDirectory = io.popen("cd"):read('*l'):gsub("\\","/") -- ugly hack, because premake tokens doesnt work -.-

require("premake5_config")
require("premake5_functions")
require("premake5_actions")
require("premake5_options")
Rebuilds = {}

DependencyDirs = {}

for k,v in pairs(DependencyNames) do
  DependencyDirs[k] = WorkspaceDirectory.. "/" ..DependenciesDir.. "/" ..v
end

BuildDirs = {}

for k,v in pairs(DependencyNames) do
  BuildDirs[k] = WorkspaceDirectory.. "/" ..BuildsDir.. "/" ..v
end

for i,v in ipairs(DependencyIncludeDirs) do
  v = DependencyDirs[i].. "/" ..v
end

for i,v in ipairs(DependencyLinkDirs) do
  v = DependencyDirs[i].. "/" ..v
  --print(v)
end

GetFilePath(WorkspaceDirectory, "README.md", false)
-- CONFIGURATIONS --
--RebuildAll = true

--Rebuilds["ASSIMP"] = true
--------------------
--DependencyLinkDirs = {}

--DependencyLinkDirs["GLM"] = ""
--DependencyLinkDirs["ASSIMP"] = ""
--DependencyLinkDirs["GLEW"] = ""
--DependencyLinkDirs["SDL2"] = ""
--DependencyLinkDirs["DevIL"] = ""
--DependencyLinkDirs["FreeType"] = ""

--DependencyIncludeDirs = {}

--DependencyIncludeDirs["GLM"] = ""
--DependencyIncludeDirs["ASSIMP"] = ""
--DependencyIncludeDirs["GLEW"] = ""
--DependencyIncludeDirs["SDL2"] = ""
--DependencyIncludeDirs["DevIL"] = ""
--DependencyIncludeDirs["FreeType"] = ""








--if RebuildAll == true then
	--printf("Rebuilding solution...")
	--os.rmdir(WorkspaceDirectory .. "/" .. BuildsDir)
	--os.mkdir(WorkspaceDirectory .. "/" .. BuildsDir)
--end

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

		--if Rebuilds["ASSIMP"] == true then
			--os.rmdir(BuildDirs[CurrentProjectConfigName])
			--os.mkdir(BuildDirs[CurrentProjectConfigName])

			--print(io.popen("cd Assets"))
			--print(io.popen("dir"))

			--io.popen("cmake --build " .. BuildDirs[CurrentProjectConfigName])
		--end

		--os.execute("cmake -E remove_directory " .. BuildDirs[CurrentProjectConfigName])
		--os.execute("cmake -E make_directory " .. BuildDirs[CurrentProjectConfigName])

		-- Linking directory...
		--if os.is64bit() then
		--	DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/lib/Release" -- This is the same for 32 and 64 bit machine, change that later.
		--else
		--	DependencyLinkDirs[CurrentProjectConfigName] = DependencyDirs[CurrentProjectConfigName] .. "/lib/Release" -- This is the same for 32 and 64 bit machine, change that later.
		--end

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
