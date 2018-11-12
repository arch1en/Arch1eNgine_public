-- CMake Functions

function CMakeGenerate(GenerateDir, CMakeListsDir, Generator, Platform)
    local log = ""
    local DefaultGenerator = "Visual Studio 15 2017"
    local DefaultPlatform = ""

    local Command = "cd /d \"" ..GenerateDir.. "\" && \"" ..AdaptDirSlashes(CMakeExecutableDir.. "/cmake\"").. " -G \""

    if Generator == "" or Generator == nil then
        Generator = DefaultGenerator
    end

    Command = Command .. Generator

    if Platform == "Win32" or Platform == "" or Platform == nil then
        Platform = DefaultPlatform
    else
        print(Platform)
        Command = Command.. " "..Platform
    end

    Command = Command.. "\" "
    Command = Command.. "\"" ..CMakeListsDir.. "\""
   
    print(Command)
    if os.target() == "windows" then
        log = io.popen(Command, 'r')
    else
        print("CMakeGenerate Error : Operational System target invalid or inoperable.")
        return
    end
    LiveLog(log)
end

function CMakeBuild(BuildDir, GeneratedDir, Configuration)
    local log = ""

    if Configuration == "Win32" then
        Configuration = ""
    end

    local Command = ""

    if os.target() == "windows" then
        Command = "cd /d " ..BuildDir.. " && cmake --build \"" ..GeneratedDir.. "\" --config \"" ..Configuration
    else
        print("CMakeBuild Error : Operational System target invalid or inoperable.")
        return
    end

    print(Command)
    log = io.popen(Command, 'r')
    LiveLog(log)
end

-- In order to make an out-of-source build, we need to remove generated cache file.
-- @todo Yet to be considered
function CMakeRemoveCacheFile(FilePath)
    os.remove(FilePath)
end

-- ~CMake Functions

function AdaptDirSlashes(String)
	local Result = String
    if os.target() == "windows" then
        Result = string.gsub(String, "/",AdaptedDirSlash())
    elseif os.target() == "linux" then
        Result = string.gsub(String, "\\",AdaptedDirSlash())
    else
        Result = string.gsub(String, "\\",AdaptedDirSlash())
    end
    return Result
end

function AdaptedDirSlash()
    if os.target() == "windows" then
        return "\\"
    elseif os.target() == "linux"  then
        return "/"
    end

     return "/"
end
-- Initialization block
DependenciesDir = "Dependencies"
ModulesDir = "Modules"
BuildsDir = "Builds"
LibrariesDir = "Libraries"
PropertiesFileName = "Properties"
DependencyBuildsDir = AdaptDirSlashes(BuildsDir.. "/" ..DependenciesDir)
DependencyLibrariesDir = AdaptDirSlashes(BuildsDir.. "/" ..LibrariesDir.. "/" ..DependenciesDir)

WorkspaceDirectory = AdaptDirSlashes(io.popen("cd"):read('*l')) -- ugly hack, because premake tokens doesnt work -.-

local GeneratedFolderName = "Generated"
local ApplicationFolderName = "Application"
local SourceFolderName = "Source"
local BuildFolderName = "Build"
local BinariesFolderName = "Binaries"
local LibrariesFolderName = "Libraries"

function GetApplicationDir()
	return AdaptDirSlashes(WorkspaceDirectory.. "/" ..ApplicationFolderName)
end

function GetBinariesDir()
    return AdaptDirSlashes(WorkspaceDirectory.. "/" ..BinariesFolderName)
end

-- Dependencies

function GetDependenciesDir()
    return AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependenciesDir)
end

function GetDependencyDir(DependencyName)
    return AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependenciesDir.. "/" ..DependencyName)
end

function GetDependencySourceDir(DependencyName)
    return AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependenciesDir.. "/" ..DependencyName.. "/" ..SourceFolderName)
end

function GetDependencyGeneratedDir(DependencyName)
    return AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependenciesDir.. "/" ..DependencyName.. "/" ..GeneratedFolderName)
end

function GetDependencyBuildDir(DependencyName)
    return AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependenciesDir.. "/" ..DependencyName.. "/" ..BuildFolderName)
end

function GetDependencyLibrariesDir(DependencyName)
    return AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependenciesDir.. "/" ..DependencyName.. "/" ..LibrariesFolderName)
end

function GetModuleDependencyLibrariesDir(ModuleName)
	return AdaptDirSlashes(WorkspaceDirectory.. "/" ..ModulesDir.. "/" ..ModuleName.. "/" ..LibrariesFolderName)
end

-- @todo Isn't it too excessive ?
function GetDependencyConfigurationMappingFromName(DependencyConfigurationName, DependencyProperties)
	for _,v in ipairs(DependencyProperties.ConfigurationProperties) do
        if v.Mapping ~= nil and v.Name == DependencyConfigurationName then
            return v.Mapping
        elseif v.Name == DependencyConfigurationName then
            return DependencyConfigurationName
        end
    end
	
	return DependencyConfigurationName
end

function GetDependencyPlatformMappingFromName(DependencyPlatformName, DependencyProperties)
	for _,v in ipairs(DependencyProperties.PlatformProperties) do
        if v.Mapping ~= nil and v.Name == DependencyPlatformName then
            return v.Mapping
        elseif v.Name == DependencyPlatformName then
            return DependencyPlatformName
        end
    end
	
	return DependencyPlatformName
end

function GetDependencyConfigurationNameFromMapping(ProjectConfiguration, DependencyProperties)
    for _,v in ipairs(DependencyProperties.ConfigurationProperties) do
        if v.Mapping ~= nil and v.Mapping == ProjectConfiguration then
            return v.Name
        elseif v.Name == ProjectConfiguration then
            return v.Name
        end
    end
end

function GetDependencyPlatformNameFromMapping(ProjectConfiguration, DependencyProperties)
    for _,v in ipairs(DependencyProperties.PlatformProperties) do
        if v.Mapping ~= nil and v.Mapping == ProjectConfiguration then
            return v.Name
        elseif v.Name == ProjectConfiguration then
            return v.Name
        end
    end
end

function MapDependencyConfigurationWithProject(ProjectConfiguration, Dependency)
    for _,v in ipairs(Dependency.ConfigurationProperties) do
        if v.Mapping ~= nil and v.Mapping == ProjectConfiguration then
            return v.Mapping
        elseif v.Name == ProjectConfiguration then
            return v.Name
        end
    end
end

function MapDependencyPlatformWithProject(ProjectPlatform, Dependency)
    for _,v in ipairs(Dependency.PlatformProperties) do
        if v.Mapping ~= nil and v.Mapping == ProjectPlatform then
            return v.Mapping
        elseif v.Name == ProjectPlatform then
            return v.Name
        end
    end
end

function GetDependencyPropertiesByName(Name)
    local Result = dofileopt(AdaptDirSlashes(GetDependenciesDir().."/"..Name.."/"..PropertiesFileName..".lua"))
    if Result == true then
        return Properties
    end

    return nil
end

function GetListOfDependencyNames()
    local Paths = os.matchfiles(AdaptDirSlashes(GetDependenciesDir().."/**/"..PropertiesFileName..".lua"))
    local DependenciesList = {}
    for i,v in pairs(Paths) do
        local Tokens = string.explode(v, "/")
        local NumberOfTokens = #Tokens
        local Name = Tokens[NumberOfTokens-1]
        if Name ~= nil then
            DependenciesList[i] = Name
        end
    end

    return DependenciesList
end

-- ~Dependencies

-- Modules

function GetModulesDir()
	return AdaptDirSlashes(WorkspaceDirectory.. "/" ..ModulesDir)
end

function GetModuleBuildDir(ModuleName)
	return AdaptDirSlashes(WorkspaceDirectory.. "/" ..ModulesDir.. "/" ..ModuleName.. "/" ..BuildFolderName)
end

function GetApplicationProperties()
    local Result = dofileopt(AdaptDirSlashes(GetApplicationDir().."/"..PropertiesFileName..".lua"))
    if Result == true then
        return Properties
    end

    return nil
end

function GetModulePropertiesByName(Name)
    local Result = dofileopt(AdaptDirSlashes(GetModulesDir().."/"..Name.."/"..PropertiesFileName..".lua"))
    if Result == true then
        return Properties
    end

    return nil
end

function GetListOfModuleNames()
    local Paths = os.matchfiles(AdaptDirSlashes(GetModulesDir().."/**/"..PropertiesFileName..".lua"))
    local DependenciesList = {}
    for i,v in pairs(Paths) do
        local Tokens = string.explode(v, "/")
        local NumberOfTokens = #Tokens
        local Name = Tokens[NumberOfTokens-1]
        if Name ~= nil then
            DependenciesList[i] = Name
        end
    end

    return DependenciesList
end

function SetupApplication()

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
    -- includedirs(WorkspaceDirectory.. "/Source") 
    
    local ApplicationProperties = GetApplicationProperties()
    if ApplicationProperties == nil then
        print("Error configuring application. Properties.lua not found !")
        return 
    end

    SetupModuleDependencies(ApplicationProperties)
    SetupForeignDependencies(ApplicationProperties)

    --for _,v in pairs(GetListOfDependencyNames()) do

    --  local DependencyProperties = GetDependencyPropertiesByName(v)
    --if DependencyProperties == nil then
    --    print("Including "..v.. " directories : Action failed. Properties not found.")
    --    return
    --end
    --local Size = #DependencyProperties.IncludeDirs
    --for j=1,Size do
    --    includedirs(AdaptDirSlashes(GetDependencyDir(DependencyProperties.Name).. "/" ..DependencyProperties.IncludeDirs[j]))
    --end
    --end

    -- Linking libraries...
    --for _,v0 in pairs(GetListOfDependencyNames()) do
    --    LinkForeignDependency(GetDependencyPropertiesByName(v0))
    --end

    --filter {}
    -- Finding libraries...
    --for _,v in pairs(GetListOfDependencyNames()) do
    --    AddForeignDependencyLibraryDirs(GetDependencyPropertiesByName(v))
    --end

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
    --pchheader "stdafx.h"
    --pchsource "stdafx.cpp"

    filter{}

end

function SetupModule(ModuleName)

    local ModuleProperties = GetModulePropertiesByName(ModuleName)
    local ModuleData
    if ModuleProperties == nil then
        print("Module "..ModuleName.. " : Cannot setup module. Properties not found.")
        return
    end

    if ModuleProperties.LinkageType == nil then
        print("Module "..ModuleName.. " : Cannot setup module. LinkageType invalid.")
        return
    end

    if ModuleProperties.LinkageType == "None" then
        -- @todo Is this really nescessary ?
        --SetupModuleDependencies(ModuleProperties)
        print("Module "..ModuleName.. " : Module without LinkageType, skipping...")
        return
    end

    print(CreateFolderIfDoesntExistEx(GetModuleBuildDir(ModuleName)))

    group("Modules")
    project(ModuleName)

    targetdir(AdaptDirSlashes(GetBinariesDir().."/%{cfg.buildcfg}/%{cfg.platform}"))
    targetname(ModuleName)
    location(AdaptDirSlashes(ModulesDir.."/"..ModuleName.."/"..BuildFolderName))
    includedirs(ModulesDir.."/"..ModuleName.."/"..SourceFolderName)
    if ModuleProperties.LinkageType == "Static" then
        kind("StaticLib")
    elseif ModuleProperties.LinkageType == "Dynamic" then
        kind("SharedLib")
        defines{"MODULE_API=__declspec(dllexport)"}
        for _,c in pairs(Configurations) do
            for _,p in pairs(Platforms) do
                filter{"configurations:"..c.Name,"platforms:"..p.Name}
                implibdir(AdaptDirSlashes(ModulesDir.."/"..ModuleName.."/"..LibrariesDir.."/"..c.Name.."/"..p.Name))
            end
        end

    end

    language("C++")

    defines{"MODULE_IMPORT=__declspec(dllimport)"}

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

    local ModuleData

    SetupDependencies("Module", ModuleProperties)
    SetupDependencies("Foreign", ModuleProperties)

    filter{"action:vs*"}
    systemversion(os.winSdkVersion() .. ".0")

    files({ModulesDir.."/"..ModuleName.."/"..SourceFolderName.."/**.h", ModulesDir.."/"..ModuleName.."/"..SourceFolderName.."/**.cpp", ModulesDir.."/"..ModuleName.."/"..SourceFolderName.."/**.inl"})

    if ModulePropertiesPostPremakeCommand ~= nil then
        ModulePropertiesPostPremakeCommand()
    end

    filter{}
end

function SetupDependencies(DependencyType, ModuleProperties)
    if ModuleProperties == nil then
        print("Module dependencies setting up failed. ModuleProperties are invalid.")
        return
    end

    if ModuleProperties.ModuleDependencies == nil or ModuleProperties.ForeignDependencies == nil then
        return
    end

    local DependencyProperties

    if DependencyType == "Foreign" then
        DependencyProperties = ModuleProperties.ForeignDependencies
    elseif DependencyType == "Module" then
        DependencyProperties = ModuleProperties.ModuleDependencies
    end

    for _,v in pairs(DependencyProperties) do
        local ModuleDependencyProperties

        Log(2, v.. " dependency.")

        if DependencyType == "Foreign" then
            ModuleDependencyProperties = GetDependencyPropertiesByName(v)
            Log(2, "Foreign Dependency Properties : " ..v)
            for _,w in ipairs(ModuleDependencyProperties.IncludeDirs) do
                includedirs(AdaptDirSlashes(GetDependencyDir(ModuleDependencyProperties.Name).."/"..w))
            end
        elseif DependencyType == "Module" then
            ModuleDependencyProperties = GetModulePropertiesByName(v)
            Log(2, "Module Dependency Properties : " ..v)
            includedirs(AdaptDirSlashes(ModulesDir.."/"..v.."/"..SourceFolderName))
        end

        if ModuleDependencyProperties == nil then
            Log(0, "Error : Cannot setup "..v.." module. Properties not found.")
            return
        end

        local LinkageType = ModuleDependencyProperties.LinkageType

        if LinkageType == "Dynamic" then
            local Result = LinkDependency(DependencyType, ModuleDependencyProperties)
            if(Result[1] == false) then
                Log(0, Result[2])
            end
            Result = AddDependencyLibraryDirs(DependencyType, ModuleDependencyProperties)
            if Result[1] == false then
                Log(0, Result[2])
            end
        end
    end
end

function SetupModuleDependencies(ModuleProperties, ModuleData)
    if ModuleProperties == nil then
        print("Module dependencies setting up failed. ModuleProperties are invalid.")
        return
    end

    if ModuleProperties.ModuleDependencies == nil then
        return
    end

    for _,v in pairs(ModuleProperties.ModuleDependencies) do
        local ModuleDependencyProperties = GetModulePropertiesByName(v)
        if ModuleDependencyProperties == nil then
            print("Error : Cannot setup "..v.." module. Properties not found.")
            return
        end

        --table.insert(ModuleData.IncludedModules, v)
        includedirs(AdaptDirSlashes(ModulesDir.."/"..v.."/"..SourceFolderName))

        if ModuleDependencyProperties.LinkageType == "Dynamic" then
            LinkDependency("Module", ModuleDependencyProperties)
            AddDependencyLibraryDirs("Module", ModuleDependencyProperties)
        end
    end
end


function SetupForeignDependencies(ModuleProperties)
    if ModuleProperties == nil then
        print("Module dependencies setting up failed. ModuleProperties are invalid.")
        return
    end

    if ModuleProperties.ForeignDependencies == nil then
        return
    end

    for _,v in pairs(ModuleProperties.ForeignDependencies) do
        local ForeignDependencyProperties = GetDependencyPropertiesByName(v)
        if ForeignDependencyProperties == nil then
            print("SetupForeignDependencies failed. "..v.." dependency property file missing.")
            break
        end
        for _,w in ipairs(ForeignDependencyProperties.IncludeDirs) do
            includedirs(AdaptDirSlashes(GetDependencyDir(ForeignDependencyProperties.Name).."/"..w))
        end

        if ForeignDependencyProperties.LinkageType == "Dynamic" then
            local Result = LinkDependency("Foreign", ForeignDependencyProperties)
            if Result[1] == false then
                Log(0, Result[2])
            end
            Result = AddDependencyLibraryDirs("Foreign", ForeignDependencyProperties)
            if Result[1] == false then
                Log(0, Result[2])
            end
        end
    end

    return { true }
end

function LinkDependency(DependencyType, DependencyProperties)
    if DependencyProperties == nil then
        return { false, "Linking failed. "..DependencyType.. " dependency properties missing." }
    end

    if DependencyType == "Foreign" then
        local PropertyGroups = DependencyProperties.PropertyGroups
        if PropertyGroups ~= nil and #PropertyGroups ~= 0 then
            for _,v1 in ipairs(PropertyGroups) do
                local ConfigurationName = GetDependencyConfigurationMappingFromName(GetPropertyGroupConfigurationName(v1.Name), DependencyProperties)
                local PlatformName = GetDependencyPlatformMappingFromName(GetPropertyGroupPlatformName(v1.Name), DependencyProperties)

                filter{"configurations:"..ConfigurationName,"platforms:"..PlatformName}

                if v1.LinkFileNames ~= nil and #v1.LinkFileNames ~= 0 then
                    for _,v2 in ipairs(v1.LinkFileNames) do
                        if v2 ~= "" then
                            links{v2}
                        end
                    end
                end
            end
        end
    elseif DependencyType == "Module" then
        for _,c in pairs(Configurations) do
            for _,p in pairs(Platforms) do
                filter{"configurations:"..c.Name,"platforms:"..p.Name}
                links{DependencyProperties.Name}
            end
        end
    else
        return { false, "Linking failed. DependencyType invalid." } 
    end
    filter{}
    return { true }
end

function AddDependencyLibraryDirs(DependencyType, DependencyProperties)
	if DependencyProperties == nil then
		return { false, "Linking : Action failed. Properties not found." }
	end
	
    if DependencyProperties.LinkageType ~= nil and DependencyProperties.LinkageType == "Dynamic" then
	
		local DependencyName = DependencyProperties.Name
		for _,c in pairs(Configurations) do
			for _,p in pairs(Platforms) do
				filter("configurations:"..c.Name,"platforms:" ..p.Name)
				local LibraryDir
				if DependencyType == "Foreign" then
					LibraryDir = AdaptDirSlashes(GetDependencyLibrariesDir(DependencyName).. "/" ..c.Name.. "/" ..p.Name)
				elseif DependencyType == "Module" then
					LibraryDir = AdaptDirSlashes(GetModuleDependencyLibrariesDir(DependencyName).. "/" ..c.Name.. "/" ..p.Name)
				else
                    return { false, "Linking "..DependencyName.." failed. DependencyType invalid." }
				end

				if os.isdir(LibraryDir) then
					libdirs(LibraryDir)
				end
			end
		end
    end
	filter{}
    return { true }
end

-- DEPRECATED
function Module_SetImportLibraryDir(ModuleName)
    local ModuleProperties = GetModulePropertiesByName(ModuleName)
    if ModuleProperties == nil then
        print("Module_SetImportLibraryDir "..ModuleName.. " : Cannot setup module. Properties not found.")
        return
    end
    -- do dokończenia
    implibdir(ModulesDir.."/"..ModuleName.."/"..Module)

end

-- ~Modules

-- os.copyfile doesnt work for windows for some reason...
function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\")
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" ..reg_arch.. "Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion")
    if sdk_version ~= nil then return sdk_version end
end

-- PropertyIndex = 0 for Configuration, 1 for Platform
function IsTokenInPropertyGroup(PropertyIndex, PropertyGroupName, Token)
    local Tokens = string.explode(PropertyGroupName, "|")
    return Tokens[PropertyIndex] == Token
end

function GetPropertyGroupConfigurationName(PropertyGroupName)
    return string.explode(PropertyGroupName, "|")[1]
end

function GetPropertyGroupPlatformName(PropertyGroupName)
    return string.explode(PropertyGroupName, "|")[2]
end

function CopyFile(Source, Destination)
    if os.target() == "windows" then
        io.popen("copy "..Source.. " " ..Destination,'r')
    else
        print("CopyFile Error : Inoperable target OS.")
    end
end

function FindDependencyByName(Args)
    setmetatable(Args, {__index={Lowercase=false}})
    local DependencyName, Lowercase =
    Args[1] or Args.DependencyName,
    Args[2] or Args.Lowercase
    for i,v in pairs(Dependencies) do
        if Lowercase == true then
            if v.Name:lower() == DependencyName:lower() then
                return i
            end
        else
            if v.Name == DependencyName then
                return i
            end
        end

    end
end

function LiveLog(log)
    repeat
        local logLine = log:read(1)
        if logLine then
            io.write(logLine) io.flush()
        end
    until not logLine
    log:close()
end

function FindDependencyLibraryFiles(DependencyProperties, ConfigurationName, PlatformName)
    local DynamicLibraryFileExtension = ""
    local LinkerLibraryFileExtension = "lib"
    if os.target() == "windows" then
        DynamicLibraryFileExtension = "dll"
    elseif os.target() == "linux" then
        DynamicLibraryFileExtension = "a"
    end

    local FilePaths =
    {
        [1] = {},
        [2] = {}
    }
    local DependencyName = DependencyProperties.Name
    local PropertyGroups = DependencyProperties.PropertyGroups
    local FoundFileNames = nil
    for i,v in pairs(PropertyGroups) do
        if IsTokenInPropertyGroup(1, v.Name, ConfigurationName) and IsTokenInPropertyGroup(2, v.Name, PlatformName) then
                FoundFileNames = v.LinkFileNames 
        end
    end 

    if FoundFileNames == nil or #FoundFileNames == 0 then
        print("Error : " ..DependencyName.. " dependency files to link were not found. Aborting.")
        return nil
    end

    for i,v in pairs(FoundFileNames) do

        local DynamicLibraryFileName = v.. "." ..DynamicLibraryFileExtension
        local LinkerLibraryFileName = v.. "." ..LinkerLibraryFileExtension
        local DynamicLibrarySearchRegex = GetDependencyGeneratedDir(DependencyName).. "/**/" ..DynamicLibraryFileName
        local LinkerLibrarySearchRegex = GetDependencyGeneratedDir(DependencyName).. "/**/" ..LinkerLibraryFileName
        local FoundFiles = os.matchfiles(DynamicLibrarySearchRegex)

        if FoundFiles[1] ~= nil then
            for j,vf in pairs(FoundFiles) do
                table.insert(FilePaths[1], vf)
            end
        end

        for i in pairs(FoundFiles) do FoundFiles[i] = nil end

        FoundFiles = os.matchfiles(LinkerLibrarySearchRegex)
        if FoundFiles[1] ~= nil then
            for j,vf in pairs(FoundFiles) do
                table.insert(FilePaths[2], vf)
            end
        end
    end
    return FilePaths
end

function CreateFolderIfDoesntExistEx(FolderPath)
    local Directory = FolderPath
    if os.isdir(Directory) == false then
        os.mkdir(Directory)
        return FolderPath.. " folder missing. Creating..."
    else
        return FolderPath.. " folder exists. Skipping creation..."
    end
end

function CreateFolderIfDoesntExist(FolderPath, FolderName)
    local Directory = FolderPath.. "/" ..FolderName
    if os.isdir(Directory) == false then
        os.mkdir(Directory)
        return FolderName.. " folder missing. Creating..."
    else
        return FolderName.. " folder exists. Skipping creation..."
    end
end

-- [DEPRECATED] Returns directory where file resids, otherwise returns empty string.
-- Yeah, ive discovered that premake5 already has a function for a file/folder search.
function GetFilePath(InitialDirectory, FileName, Recursive)
    if InitialDirectory == nil then
        print("Error : InitialDirectory argument is invalid.")
        return ""
    elseif FileName:match('%w+%.%w+') == nil then
        print("Error : " ..FileName.. " is a wrong FileName argument.")
        return ""
    end

    local command = ""

    -- Windows variant.
    if os.target() == "windows" then
        InitialDirectory = AdaptDirSlashes(InitialDirectory)
        command = "dir " ..InitialDirectory.. " /b"
    end

    local result = io.popen(command)
    for file in result:lines() do
        if file:match('(' ..FileName.. ')') ~= nil then -- File found
            --print("Found : " ..file:match('%w+%.%w+'))
            result:close()
            return InitialDirectory
        elseif file:match('^[^.]*$') ~= nil then  -- We have a directory
            --print("Dir : " ..file:match('.+[^%.$w+]'))

            if Recursive == true then
                local FilePath = GetFilePath(AdaptDirSlashes(InitialDirectory.. "/" ..file), FileName, true)
                if FilePath ~= "" then
                    --print("Recursion Found : " ..FileName.. " in " ..FilePath)
                    result:close()
                    return FilePath
                end
            end

        else -- We have a file
            --print("File : " ..file)
        end
    end
    result:close()
    return ""
end

-- GNU Makefile Functions
function MakefileGenerate(BuildDir, DependencyDir)
    local makeExecutable = ""
    if os.matchfiles(AdaptDirSlashes(MakefileExecutableDir.. "/").. "make.exe")[1] ~= nil then
        makeExecutable = "make"
    elseif os.matchfiles(AdaptDirSlashes(MakefileExecutableDir.. "/").. "mingw32-make.exe")[1] ~= nil then
        makeExecutable = "mingw32-make"
    end
    local log = ""

    if os.istarget("windows") then
        log = io.popen("cd /d " .. BuildDir .. " && " ..AdaptDirSlashes(MakefileExecutableDir.. "/").. makeExecutable.. " -C" .. DependencyDir)
    elseif os.istarget("linux") then

    end
    LiveLog(log)
end

function MakefileBuild(BuildDir, DependencyDir)

end

function CleanAllDependencies()
    for _,v in ipairs(GetListOfDependencyNames()) do
        CleanDependency(v)
    end
end

function GenerateAllDependencies()
    for _,v in ipairs(GetListOfDependencyNames()) do
        GenerateDependency(v)
    end
end

function BuildAllDependencies()
    for _,v in ipairs(GetListOfDependencyNames()) do
        BuildDependency(v,_OPTIONS["configuration"], _OPTIONS["platform"])
    end
end

function OrganizeAllDependencies()
    for _,v in ipairs(GetListOfDependencyNames()) do
        OrganizeDependency(v, _OPTIONS["configuration"], _OPTIONS["platform"])
    end
end

-- Returns name of the tool, that this dependency is using.
function DetermineDependencyBuildTool(DependencyName)
    if os.matchfiles(GetDependencySourceDir(DependencyName).. "/CMakeLists.txt")[1] ~= nil then
        return "cmake"
    elseif os.matchfiles(GetDependencySourceDir(DependencyName).. "/Makefile")[1] ~= nil then
        return "makefile"
    end
    return ""
end

function CleanDependency(DependencyName)
    local DependencyProperties = GetDependencyPropertiesByName(DependencyName)
    if DependencyProperties == nil then
        print("Clean : Error, cannot clean "..DependencyName.." dependency. Properties not found.")
        return
    end

    if DependencyName == "" then
        print("Error : Cannot clean " ..DependencyName.. " option.")
        return
    end

    if os.isdir(GetDependencyGeneratedDir(DependencyName)) then
        print("Clean : Cleaning " ..DependencyName.. " removing " ..GeneratedFolderName.. " directory.")
        os.rmdir(GetDependencyGeneratedDir(DependencyName))
    else
        print("Clean : " ..GeneratedFolderName.. " directory for " ..DependencyName.. " inaccessible or cleaned already.")
    end

    if os.isdir(GetDependencyBuildDir(DependencyName)) then
        print("Clean : Cleaning " ..DependencyName.. " removing " ..BuildFolderName.. " directory.")
        os.rmdir(GetDependencyBuildDir(DependencyName))
    else
        print("Clean : " ..BuildFolderName.. " directory for " ..DependencyName.. " inaccessible or cleaned already.")
    end

    if os.isdir(GetDependencyLibrariesDir(DependencyName)) then
        print("Clean : Cleaning " ..DependencyName.. " removing " ..LibrariesFolderName.. " directory.")
        os.rmdir(GetDependencyLibrariesDir(DependencyName))
    else
        print("Clean : " ..LibrariesFolderName.. " directory for " ..DependencyName.. " inaccessible or cleaned already.")
    end
end

function GenerateDependency(DependencyName)
    local DependencyProperties = GetDependencyPropertiesByName(DependencyName)
    if DependencyProperties == nil then
        print("Generate : Error generating "..DependencyName..". Properties not found.")
        return
    end

    if DependencyProperties.RequiresGeneration == false then
        print("Generate : " ..DependencyProperties.Name.. " doesn't require generation. Skipping...")
        return
    end
    local Dependency = DependencyProperties
    local DependencyName = DependencyProperties.Name
    if DependencyName == "" then
        print("Error : Cannot generate option with index " ..DependencyIndex.. ".")
        return
    end

    local GenerationTool = DetermineDependencyBuildTool(DependencyName)

    print(GenerationTool)
    print("Generate : Generating " ..DependencyName)

    print("Generate : " ..CreateFolderIfDoesntExist(GetDependencyDir(DependencyName), GeneratedFolderName))

    -- @todo This thing should be changed on some kind of system that determines the generator type.
    local GeneratorType = "Visual Studio 15 2017" 

    if GenerationTool == "cmake" then
        CMakeGenerate(GetDependencyGeneratedDir(DependencyName), GetDependencySourceDir(DependencyName), GeneratorType, _OPTIONS["platform"])
    elseif GenerationTool == "makefile" then
        MakefileGenerate(GetDependencyGeneratedDir(DependencyName), GetDependencySourceDir(DependencyName), GeneratorType, MappedDependencyPlatform)
    end
end

function BuildDependency(DependencyName, Configuration, Platform)

    if DependencyName == nil then
        print("Error : Cannot build " ..DependencyName.. " option.")
        return
    end

    local DependencyProperties = GetDependencyPropertiesByName(DependencyName)
    if DependencyProperties == nil then
        print("Build : Error, cannot build "..DependencyName.." dependency. Properties not found.")
        return
    end

    if DependencyProperties.RequiresBuilding == false or DependencyProperties.ConfigurationProperties == nil then
        print("Build : " ..DependencyName.. " doesn't require building. Skipping...")
        return
    end

    local BuildTool = DetermineDependencyBuildTool(DependencyName)
    local MappedDependencyConfiguration = GetDependencyConfigurationNameFromMapping(Configuration, DependencyProperties)
    local MappedDependencyPlatform = GetDependencyPlatformNameFromMapping(Platform, DependencyProperties)

    if BuildTool == "cmake" then
        for _,v in pairs(DependencyProperties.ConfigurationProperties) do
            print("Build : " ..CreateFolderIfDoesntExist(GetDependencyDir(DependencyName), BuildFolderName))
            CMakeBuild(GetDependencyBuildDir(DependencyName), GetDependencyGeneratedDir(DependencyName), MappedDependencyPlatform ,MappedDependencyConfiguration)
        end
    elseif BuildTool == "make" then
        -- @todo Makefile build needs to be implemented.
        print("Makefile build needs to be implemented.")
        --MakefileBuild(DependenciesBuildDirs[Dependency], DependencyDirs[Dependency])
    end

end

function IsOrganizeable()
    for _,v in pairs(Configurations) do
        if v.Name then
            return true
        end
    end
    return false
end

function OrganizeDependency(DependencyName, ConfigurationName, PlatformName)
    local DependencyProperties = GetDependencyPropertiesByName(DependencyName)
    if DependencyProperties == nil then
        print("Organize : Error, cannot organize "..DependencyName.." dependency. Properties not found.")
        return
    end

    if DependencyProperties.PropertyGroups == nil then
        print("Organize : " ..DependencyProperties.Name.. " has no property groups. Leaving...")
        return
    end
    for _,v0 in pairs(DependencyProperties.PropertyGroups) do
        if #v0.LinkFileNames == 0 then
            print("Organize : " ..DependencyProperties.Name.. " in " ..v0.Name.. " configuration have no files to link.")
            break
        end
    end

    local FoundFiles = FindDependencyLibraryFiles(DependencyProperties, ConfigurationName, PlatformName)

    --Move library files to the Builds/Libraries folder, and Dynamic Libraries to the Binaries folder.
    if FoundFiles == nil or #FoundFiles[1] == 0 or #FoundFiles[2] == 0 then
        print(DependencyProperties.Name.. " Organize : Error, no libraries found.")
        return
    end

    local DependencyName = DependencyProperties.Name
    print("Organize : " ..CreateFolderIfDoesntExist(GetDependencyDir(DependencyName), LibrariesFolderName))

    -- Copy dynamic link files (dll and a) to respective folder.
    for _,v in pairs(FoundFiles[1]) do
        local BinaryFolderName = AdaptDirSlashes(GetBinariesDir().. "/" ..ConfigurationName.. "/" ..PlatformName)
        print("Organize : " ..CreateFolderIfDoesntExistEx(BinaryFolderName))
        CopyFile(AdaptDirSlashes(v), BinaryFolderName)
        --print(os.copyfile(AdaptDirSlashes(v), AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependencyLibrariesDir.. "/" ..Dependency)))
    end
    -- Copy linker files (lib) to respective folder.
    for _,v in pairs(FoundFiles[2]) do
        local DependencyLibraryFolderName = AdaptDirSlashes(GetDependencyLibrariesDir(DependencyName).. "/" ..ConfigurationName.. "/" ..PlatformName)
        print("Organize : " ..CreateFolderIfDoesntExistEx(DependencyLibraryFolderName))
        CopyFile(AdaptDirSlashes(v), DependencyLibraryFolderName)
        --print(os.copyfile(AdaptDirSlashes(v), AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependencyLibrariesDir.. "/" ..Dependency)))
    end
end
