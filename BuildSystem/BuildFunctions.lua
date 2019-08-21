-- CMake Functions

require("GlobalFunctions")
require("UserConfigHandler")

function CMakeGenerate(GenerateDir, CMakeListsDir, Platform)
    local log = ""
    local DefaultGenerator = "Visual Studio 16 2019"
    local DefaultPlatform = ""
    local DefaultToolsetVersion = "v141"

    local uch = UserConfigHandler.New()
    local CMakeUserData = UserConfigHandler.FindDepedencyDataByName(uch, "CMake")
    local CMakeExecutableDir = CMakeUserData.Properties.Directory
    local Generator = CMakeUserData.Properties.DefaultGenerator
    local ToolsetVersion = CMakeUserData.Properties.DefaultToolsetVersion

    local Command = "cd /d \"" ..GenerateDir.. "\" && \"" ..AdaptDirSlashes(CMakeExecutableDir.. "/bin/cmake\"").. " -G \""

    --Generator
    if Generator == "" or Generator == nil then
        Log(1, "No CMake Generator found. Using default one ("..DefaultGenerator..").")
        Generator = DefaultGenerator
    end

    Command = Command .. Generator

    -- Platform
    if Platform == "Win32" or Platform == "" or Platform == nil then
        Log(1, "No concrete platform found. Generating from default platform.")
        Platform = DefaultPlatform
    end
 
    Command = Command.. "\" -A "..Platform

    --Toolset Version
    if ToolsetVersion == "" or ToolsetVersion == nil then
        Log(1, "No toolset version found. Generating with default toolset version ("..DefaultToolsetVersion..").")
        ToolsetVersion = DefaultToolsetVersion
    end

    Command = Command.." -T "..ToolsetVersion.." "
    Command = Command.. "\"" ..CMakeListsDir.. "\""

    Log(0,Command)
    if os.target() == "windows" then
        log = io.popen(Command, 'r')
    else
        Log(0,"CMakeGenerate Error : Operational System target invalid or inoperable.")
        return
    end
    LiveLog(log)
end

function CMakeBuild(BuildDir, GeneratedDir, Configuration)

    local log = ""
    local Command = ""
    local DefaultConfiguration = "Debug"

    if Configuration == "" or Configuration == nil then
        Log(1, "No valid configuration passed. Using default configuration ("..DefaultConfiguration..").")
        Configuration = DefaultConfiguration
    end

    if os.target() == "windows" then
        Command = "cd /d " ..BuildDir.. " && cmake --build \"" ..GeneratedDir.. "\" --config "..Configuration
    else
        Log(0, "CMakeBuild Error : Operational System target invalid or inoperable.")
        return
    end

    Log(0,Command)
    log = io.popen(Command, 'r')
    LiveLog(log)
end

-- In order to make an out-of-source build, we need to remove generated cache file.
-- @todo Yet to be considered
function CMakeRemoveCacheFile(FilePath)
    os.remove(FilePath)
end

-- ~CMake Functions

-- Initialization block

-- Dependencies

-- @todo Isn't it too excessive ?
function GetDependencyConfigurationMappingFromName(DependencyConfigurationName, DependencyProperties)

    local ConfigurationProperties = DependencyProperties.ConfigurationProperties

    if ConfigurationProperties ~= nil and #ConfigurationProperties ~= 0 then
        for _,v in ipairs(ConfigurationProperties) do
            if v.Mapping ~= nil and v.Name == DependencyConfigurationName then
                return v.Mapping
            elseif v.Name == DependencyConfigurationName then
                return DependencyConfigurationName
            end
        end
    end

    return DependencyConfigurationName
end

function GetDependencyPlatformMappingFromName(DependencyPlatformName, DependencyProperties)

    local PlatformProperties = DependencyProperties.PlatformProperties

    if PlatformProperties ~= nil and #PlatformProperties ~= 0 then
        for _,v in ipairs(DependencyProperties.PlatformProperties) do
            if v.Mapping ~= nil and v.Name == DependencyPlatformName then
                return v.Mapping
            elseif v.Name == DependencyPlatformName then
                return DependencyPlatformName
            end
        end
    end
	return DependencyPlatformName
end

function GetDependencyConfigurationNameFromMapping(ProjectConfiguration, DependencyProperties)
    for _,v in ipairs(DependencyProperties.ConfigurationProperties) do
        if v.Mapping ~= nil and v.Mapping == ProjectConfiguration then -- There is mapping.
            return v.Name
        elseif v.Name == ProjectConfiguration then -- There is no mapping, but Dependency Configuration has the same name.
            return v.Name
        end
    end

    return nil  -- There is no mapping and configuration at all.
end

function GetDependencyPlatformNameFromMapping(ProjectConfiguration, DependencyProperties)
    
    local PlatformProperties = DependencyProperties.PlatformProperties

    if PlatformProperties ~= nil and #PlatformProperties ~= 0 then
        for _,v in ipairs(PlatformProperties) do
            if v.Mapping ~= nil and v.Mapping == ProjectConfiguration then -- There is mapping.
                return v.Name
            elseif v.Name == ProjectConfiguration then -- There is no mapping, but Dependency Platform has the same name.
                return v.Name
            end 
        end
    else
        Log(0, "Getting dependency platform name from mapping failed. No platform properties found.")
    end

    Log(0, "No platform mapping found.")
	return nil -- There is no mapping and platform at all.
end

function GetDependencyPropertiesByName(Name)
    local Result = dofileopt(AdaptDirSlashes(GetDependenciesDir().."/"..Name.."/"..GetPropertiesFileName()..".lua"))
    if Result == true then
        return Properties
    end

    return nil
end

function GetListOfDependencyNames()
    local Paths = os.matchfiles(AdaptDirSlashes(GetDependenciesDir().."/**/"..GetPropertiesFileName()..".lua"))
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

function GetApplicationProperties()
    local Result = dofileopt(AdaptDirSlashes(GetApplicationDir().."/"..GetPropertiesFileName()..".lua"))
    if Result == true then
        return Properties
    end

    return nil
end

function GetModulePropertiesByName(Name)
    local Result = dofileopt(AdaptDirSlashes(GetModulesDir().."/"..Name.."/"..GetPropertiesFileName()..".lua"))
    if Result == true then
        return Properties
    end

    return nil
end

function GetListOfModuleNames()
    local Paths = os.matchfiles(AdaptDirSlashes(GetModulesDir().."/**/"..GetPropertiesFileName()..".lua"))
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

function SetupModule(ModuleName)

    local ModuleProperties = GetModulePropertiesByName(ModuleName)
    local ModuleData
	local IncludeDir = GetModulesDir().."/"..ModuleName.."/"..GetSourceFolderName()
	local FilesToAdd = {GetModulesDir().."/"..ModuleName.."/"..GetSourceFolderName().."/**.h", GetModulesDir().."/"..ModuleName.."/"..GetSourceFolderName().."/**.cpp", GetModulesDir().."/"..ModuleName.."/"..GetSourceFolderName().."/**.inl"}
	
    if ModuleProperties == nil then
        Log(0, "Module "..ModuleName.. " : Cannot setup module. Properties not found.")
        return
    end

    if ModuleProperties.LinkageType == nil then
        Log(0, "Module "..ModuleName.. " : Cannot setup module. LinkageType invalid.")
        return
    end

	group("Modules")
	
    if ModuleProperties.LinkageType == "None" then
        -- @todo Is this really nescessary ?
        Log(1, "Module "..ModuleName.. " : Module without LinkageType, skipping...")
        return
    elseif ModuleProperties.LinkageType == "Include" then
		files(FilesToAdd)
		return
	end
	
    Log(0,CreateFolderIfDoesntExistEx(GetModuleBuildDir(ModuleName)))

    
    project(ModuleName)

    targetdir(AdaptDirSlashes(GetBinariesDir().."/%{cfg.buildcfg}/%{cfg.platform}"))
    targetname(ModuleName)
    location(AdaptDirSlashes(ModulesDir.."/"..ModuleName.."/"..GetBuildFolderName()))
    includedirs(IncludeDir)
	
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

	else
		defines{"MODULE_API=__declspec(dllimport)"}
    end

	filter{}

    language("C++")

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

    filter{}
end

function SetupDependencies(DependencyType, ModuleProperties)
    if ModuleProperties == nil then
        Log(0,"Module dependencies setting up failed. ModuleProperties are invalid.")
        return
    end

    local DependencyProperties

    if DependencyType == "Foreign" and ModuleProperties.ForeignDependencies ~= nil then
        DependencyProperties = ModuleProperties.ForeignDependencies
    elseif DependencyType == "Module" and ModuleProperties.ModuleDependencies ~= nil then
        DependencyProperties = ModuleProperties.ModuleDependencies
    else
		Log(1, "No "..DependencyType.." dependencies.")
		return
	end

    for _,v in pairs(DependencyProperties) do
        local ModuleDependencyProperties

        Log(2, v.. " dependency.")

        if DependencyType == "Foreign" then
            ModuleDependencyProperties = GetDependencyPropertiesByName(v)
            Log(2, "Foreign Dependency Properties : " ..v)
        elseif DependencyType == "Module" then
            ModuleDependencyProperties = GetModulePropertiesByName(v)
            Log(2, "Module Dependency Properties : " ..v)
        end

        if ModuleDependencyProperties == nil then
            Log(0, "Error : Cannot setup "..v.." module. Properties not found.")
            return
        end

        IncludeDependency(DependencyType, ModuleDependencyProperties)
        local LinkageType = ModuleDependencyProperties.LinkageType

        if LinkageType == "Dynamic" then
            local Result = LinkDependency(DependencyType, ModuleDependencyProperties)
            if Result[1] == false then
                Log(0, Result[2])
            end
            
			AddDependencyLibraryDirs(DependencyType, ModuleDependencyProperties)
   
        end
		
		if ModuleDependencyProperties.PostPremakeCommand ~= nil then
			ModuleDependencyProperties.PostPremakeCommand()
			filter{}
		end
    end
end

function IncludeDependency(DependencyType, DependencyProperties)
    
    local DependencyDir = ""
    local DependencyName = DependencyProperties.Name

    if DependencyType == "Module" then
        includedirs(AdaptDirSlashes(GetModulesDir().."/"..DependencyName.."/"..GetSourceFolderName()))
    elseif DependencyType == "Foreign" then
        if DependencyProperties.UserConfig ~= nil and DependencyProperties.UserConfig.PathData ~= nil then
            Log(2, DependencyProperties.Name .." dependency user config path data found.")
            local uch = UserConfigHandler.New()
            local DependencyUserData = UserConfigHandler.FindDepedencyDataByName(uch, DependencyProperties.Name)
			 
			 if DependencyUserData == nil then
                Log(1, "Dependency user data not found. Config file might be not created yet.")
				return
            end
			
            local DependencyUserDir = DependencyUserData.Properties.Directory

            if DependencyUserDir ~= nil then
                DependencyDir = DependencyUserDir
                Log(2, "Dependency directory set to : "..DependencyDir)
            end

        else
            DependencyDir = GetDependencyDir(DependencyName)
        end

        for _,w in ipairs(DependencyProperties.IncludeDirs) do
            includedirs(AdaptDirSlashes(DependencyDir.."/"..w))
        end
    end
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
		else
            Log(0, "Dependency linking failed. No property groups set")
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
		Log(1, "Linking : Action failed. Properties not found.")
		return
	end
	
    if DependencyProperties.LinkageType ~= nil and DependencyProperties.LinkageType == "Dynamic" then
	
		local DependencyName = DependencyProperties.Name
		for _,c in pairs(Configurations) do
			for _,p in pairs(Platforms) do

				filter("configurations:"..c.Name,"platforms:" ..p.Name)
				
                local LibraryDir = ""
				if DependencyType == "Foreign" then
                    if DependencyProperties.UserConfig ~= nil and DependencyProperties.UserConfig.PathData ~= nil then
                        local uch = UserConfigHandler.New()
						local DependencyUserData = UserConfigHandler.FindDepedencyDataByName(uch, DependencyProperties.Name)
			 
						if DependencyUserData == nil then
							filter{}
							Log(1, "Dependency user data not found. Config file might be not created yet.")
							return false
						end
			
						local DependencyUserDir = DependencyUserData.Properties.Directory

						if DependencyUserDir ~= nil then
							Log(2, "Dependency directory set to : "..DependencyUserDir)
						end
                
                        local FoundPropertyGroup = FindPropertyGroup(DependencyProperties.PropertyGroups, c.Name, p.Name)
                        
                        if FoundPropertyGroup ~= nil then
                            local PropertyLibraryDir = FoundPropertyGroup.LibraryDir
                            if PropertyLibraryDir == nil then
                                Log(0, "Adding dependency library directories failed. LibraryDir property is invalid.")
                            else
                                LibraryDir = AdaptDirSlashes(DependencyUserDir.."/"..PropertyLibraryDir)
                                Log(2, "PropertyGroup for "..c.Name.."|"..p.Name.." found.")
                            end
                        else
                            Log(1, "No PropertyGroup for "..c.Name.."|"..p.Name.." found. Skipping...")
                        end

                    else
					    LibraryDir = AdaptDirSlashes(GetDependencyLibrariesDir(DependencyName).. "/" ..c.Name.. "/" ..p.Name)
                    end
				elseif DependencyType == "Module" then
					LibraryDir = AdaptDirSlashes(GetModuleDependencyLibrariesDir(DependencyName).. "/" ..c.Name.. "/" ..p.Name)
				else
					Log(0, "Linking "..DependencyName.." failed. DependencyType invalid.")
                    return
				end
    
				if LibraryDir ~= "" and os.isdir(LibraryDir) then
					libdirs(LibraryDir)
				end
			end
		end
    end
	filter{}
end

-- DEPRECATED
function Module_SetImportLibraryDir(ModuleName)
    local ModuleProperties = GetModulePropertiesByName(ModuleName)
    if ModuleProperties == nil then
        Log(0,"Module_SetImportLibraryDir "..ModuleName.. " : Cannot setup module. Properties not found.")
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
function FindPropertyGroup(InPropertyGroups, ConfigurationName, PlatformName)
    for i,v in pairs(InPropertyGroups) do
        local Tokens = DetokenizePropertyGroup(v.Name)
        if Tokens[1] == ConfigurationName and Tokens[2] == PlatformName then
                return v 
        end
    end 

    return nil
end

function DetokenizePropertyGroup(PropertyGroupName)
    local Tokens = string.explode(PropertyGroupName, "|")
    return Tokens
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
        Log(0,"CopyFile Error : Inoperable target OS.")
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

    local DependencyPlatformName = GetDependencyPlatformNameFromMapping(PlatformName, DependencyProperties)

    if DependencyPlatformName ~= nil then
        PlatformName = DependencyPlatformName
    end

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
    local FoundPropertyGroup = FindPropertyGroup(PropertyGroups, ConfigurationName, PlatformName)

    if FoundPropertyGroup ~= nil then
        FoundFileNames = FoundPropertyGroup.LinkFileNames
    end
	
    if FoundFileNames == nil or #FoundFileNames == 0 then
        Log(0,"Error : " ..DependencyName.. " dependency files to link were not found. Aborting.")
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
        Log(0,"Error : InitialDirectory argument is invalid.")
        return ""
    elseif FileName:match('%w+%.%w+') == nil then
        Log(0,"Error : " ..FileName.. " is a wrong FileName argument.")
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
            Log(3,"Found : " ..file:match('%w+%.%w+'))
            result:close()
            return InitialDirectory
        elseif file:match('^[^.]*$') ~= nil then  -- We have a directory
            Log(3,"Dir : " ..file:match('.+[^%.$w+]'))

            if Recursive == true then
                local FilePath = GetFilePath(AdaptDirSlashes(InitialDirectory.. "/" ..file), FileName, true)
                if FilePath ~= "" then
                    Log(3,"Recursion Found : " ..FileName.. " in " ..FilePath)
                    result:close()
                    return FilePath
                end
            end

        else -- We have a file
            --Log(0,"File : " ..file)
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
	CurrentAction = "Clean"
    if DependencyProperties == nil then
        LogAct(0, DependencyName, "Error, cannot clean "..DependencyName.." dependency. Properties not found.")
        return
    end

    if DependencyName == "" then
        LogAct(0, DependencyName, "Cannot clean " ..DependencyName.. " option.")
        return
    end

    if os.isdir(GetDependencyGeneratedDir(DependencyName)) then
        LogAct(0, DependencyName, "Cleaning " ..DependencyName.. " removing " ..GetGeneratedFolderName().. " directory.")
        os.rmdir(GetDependencyGeneratedDir(DependencyName))
    else
        LogAct(0, DependencyName, GetGeneratedFolderName().. " directory for " ..DependencyName.. " inaccessible or cleaned already.")
    end

    if os.isdir(GetDependencyBuildDir(DependencyName)) then
        LogAct(0, DependencyName, "Cleaning " ..DependencyName.. " removing " ..GetBuildFolderName().. " directory.")
        os.rmdir(GetDependencyBuildDir(DependencyName))
    else
        LogAct(0, DependencyName, GetBuildFolderName().. " directory for " ..DependencyName.. " inaccessible or cleaned already.")
    end

    if os.isdir(GetDependencyLibrariesDir(DependencyName)) then
        LogAct(0, DependencyName, "Cleaning " ..DependencyName.. " removing " ..GetLibrariesFolderName().. " directory.")
        os.rmdir(GetDependencyLibrariesDir(DependencyName))
    else
        LogAct(0, DependencyName, GetLibrariesFolderName().. " directory for " ..DependencyName.. " inaccessible or cleaned already.")
    end
end

function GenerateDependency(DependencyName)
    local DependencyProperties = GetDependencyPropertiesByName(DependencyName)
	CurrentAction = "Generate"
    if DependencyProperties == nil then
        LogAct(0, DependencyName, "Error generating "..DependencyName..". Properties not found.")
        return
    end

    if DependencyProperties.RequiresGeneration == false then
        LogAct(0, DependencyName, DependencyProperties.Name.. " doesn't require generation. Skipping...")
        return
    end
    local Dependency = DependencyProperties
    local DependencyName = DependencyProperties.Name
    if DependencyName == "" then
        LogAct(0, DependencyName, "Error : Cannot generate option with index " ..DependencyIndex.. ".")
        return
    end

    local GenerationTool = DetermineDependencyBuildTool(DependencyName)

    LogAct(0, DependencyName, GenerationTool)
    LogAct(0, DependencyName, "Generating " ..DependencyName)

    LogAct(0, DependencyName, "Generate : " ..CreateFolderIfDoesntExist(GetDependencyDir(DependencyName), GetGeneratedFolderName()))

    local Platform = GetDependencyPlatformNameFromMapping(_OPTIONS["platform"], DependencyProperties)

    if GenerationTool == "cmake" then
        CMakeGenerate(GetDependencyGeneratedDir(DependencyName), GetDependencySourceDir(DependencyName), Platform)
    elseif GenerationTool == "makefile" then
        MakefileGenerate(GetDependencyGeneratedDir(DependencyName), GetDependencySourceDir(DependencyName), Platform)
    end
end

function BuildDependency(DependencyName, Configuration, Platform)
	CurrentAction = "Build"

    if DependencyName == nil then
        LogAct(0, DependencyName, "Error : Cannot build " ..DependencyName.. " option.")
        return
    end

    local DependencyProperties = GetDependencyPropertiesByName(DependencyName)
    if DependencyProperties == nil then
        LogAct(0, DependencyName, "Build : Error, cannot build "..DependencyName.." dependency. Properties not found.")
        return
    end

    if DependencyProperties.RequiresBuilding == false or DependencyProperties.ConfigurationProperties == nil then
        LogAct(0, DependencyName, "Build : " ..DependencyName.. " doesn't require building. Skipping...")
        return
    end

    local BuildTool = DetermineDependencyBuildTool(DependencyName)
    local MappedDependencyConfiguration = GetDependencyConfigurationNameFromMapping(Configuration, DependencyProperties)
    local MappedDependencyPlatform = GetDependencyPlatformNameFromMapping(Platform, DependencyProperties)
	
	if MappedDependencyConfiguration == nil then
		LogAct(0, DependencyName, "Missing \""..Configuration.."\" configuration. Aborting...")
		return
	end

	if MappedDependencyPlatform == nil then
		LogAct(0, DependencyName, "Missing \""..Platform.."\" platform. Aborting...")
		return
	end

    if BuildTool == "cmake" then
        for _,v in pairs(DependencyProperties.ConfigurationProperties) do
            Log(0,"Build ["..DependencyName.."] : " ..CreateFolderIfDoesntExist(GetDependencyDir(DependencyName), GetBuildFolderName()))
            CMakeBuild(GetDependencyBuildDir(DependencyName), GetDependencyGeneratedDir(DependencyName), MappedDependencyConfiguration)
        end
    elseif BuildTool == "make" then
        -- @todo Makefile build needs to be implemented.
        Log(0,"Makefile build needs to be implemented.")
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
	CurrentAction = "Organize"
    if DependencyProperties == nil then
        LogAct(0, DependencyName, "Organize : Error, cannot organize "..DependencyName.." dependency. Properties not found.")
        return
    end

    if DependencyProperties.PropertyGroups == nil then
        LogAct(0, DependencyName, "Organize : " ..DependencyProperties.Name.. " has no property groups. Leaving...")
        return
    end
    for _,v0 in pairs(DependencyProperties.PropertyGroups) do
        if #v0.LinkFileNames == 0 then
            LogAct(0, DependencyName, "Organize : " ..DependencyProperties.Name.. " in " ..v0.Name.. " configuration have no files to link.")
            break
        end
    end

    local FoundFiles = FindDependencyLibraryFiles(DependencyProperties, ConfigurationName, PlatformName)

    --Move library files to the Builds/Libraries folder, and Dynamic Libraries to the Binaries folder.
    if FoundFiles == nil or #FoundFiles[1] == 0 or #FoundFiles[2] == 0 then
        LogAct(0, DependencyName, DependencyProperties.Name.. " Organize : Error, no libraries found.")
        return
    end

    local DependencyName = DependencyProperties.Name
    LogAct(0, DependencyName, CreateFolderIfDoesntExist(GetDependencyDir(DependencyName), GetLibrariesFolderName()))

    -- Copy dynamic link files (dll and a) to respective folder.
    for _,v in pairs(FoundFiles[1]) do
        local BinaryFolderName = AdaptDirSlashes(GetBinariesDir().. "/" ..ConfigurationName.. "/" ..PlatformName)
        LogAct(0, DependencyName, CreateFolderIfDoesntExistEx(BinaryFolderName))
        CopyFile(AdaptDirSlashes(v), BinaryFolderName)
        --Log(0,os.copyfile(AdaptDirSlashes(v), AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependencyLibrariesDir.. "/" ..Dependency)))
    end
    -- Copy linker files (lib) to respective folder.
    for _,v in pairs(FoundFiles[2]) do
        local DependencyLibraryFolderName = AdaptDirSlashes(GetDependencyLibrariesDir(DependencyName).. "/" ..ConfigurationName.. "/" ..PlatformName)
        LogAct(0, DependencyName, CreateFolderIfDoesntExistEx(DependencyLibraryFolderName))
        CopyFile(AdaptDirSlashes(v), DependencyLibraryFolderName)
        --Log(0,os.copyfile(AdaptDirSlashes(v), AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependencyLibrariesDir.. "/" ..Dependency)))
    end
end



function FindApplication(FileIdentifier, DefaultPaths)

	-- Prepare command sufix and prefix.
	local CommandSamplePrefix, CommandSamplePostfix
	if os.target() == "windows" then
		CommandSamplePrefix = "dir "
		CommandSamplePostfix = " /b"
	end
	
	-- Recursive search.
	for i,v in pairs(DefaultPaths) do
		-- Prepare commands.
		local Command = CommandSamplePrefix .. AdaptDirSlashes(v) .. CommandSamplePostfix
		
			local pfile = io.popen(Command)
			for line in pfile:lines() do
			
				local FullPath = DefaultPaths[i].. line
				if os.isdir(FullPath) then
					local FullPathTable = {}
					FullPathTable[0] = FullPath .. "/"
					Found = FindApplication(FileIdentifier, FullPathTable)
					if Found ~= nil then
						pfile:close()
						return Found
					end
				elseif os.isfile(FullPath) then
					IsFound = string.find(FullPath, FileIdentifier)
					if IsFound ~= nil then
						pfile:close()
						return FullPath
					end
				end
			end
			pfile:close()
	end
	
	return nil
end
