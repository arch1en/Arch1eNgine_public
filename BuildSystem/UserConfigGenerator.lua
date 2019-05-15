
require("GlobalFunctions")

UserConfig = {

}

ToolField = 
{
	
}

EDependencyType =
{
	Dependency = "Dependency",
	Module = "Module",
	Tool = "Tool"
}

function UserConfig.New(self, o)
	o = o or {}
	local self = setmetatable(o, UserConfig)
	self.__index = o
	
	return o
end

function UserConfig.GetDependencyInstallDir(self, Properties)

	local PathData = nil
		
	if os.target() == "windows" then
		PathData = Properties.PathData.Win32
	end
		
	Path = UserConfig.GetRegistryValue(self, PathData.Registry)

	if Path == nil or Path == "" then
		
		for k,v in ipairs(PathData.Defaults) do
			
			local MatchResult = os.matchdirs(AdaptDirSlashes(v))
				
			if MatchResult[1] ~= nil then
				
				return MatchResult[1]
					
			end
		end
	end
	
	return Path
end

function UserConfig.GetDependencyProperties(self, DependencyTypeEnum, Name)
	local DependenciesDir = UserConfig.GetDependencyDirByType(self, DependencyTypeEnum)
	local Path = os.matchfiles(AdaptDirSlashes(DependenciesDir.."/"..Name.."/"..GetPropertiesFileName()..".lua"))
	if Path ~= nil then
		return GetPropertiesFromFile(Path[1])
	end
end

function UserConfig.GetRegistryValue(self, RegistryPath)
	local RegistryResult = ""
	
	if os.target() == "windows" then
		RegistryResult = os.getWindowsRegistry(RegistryPath)
	end
	
	return RegistryResult
end

function UserConfig.GenerateUserConfigFile(self)

	local Names = UserConfig.GetDependencyNamesByType(self, EDependencyType.Tool)
	
	local ConfigFileBody = {}
	local Success = true
	local DependencyType = EDependencyType.Tool
	for _,v in ipairs(Names) do
		local Properties = UserConfig.GetDependencyProperties(self, DependencyType, v)
		if Properties == nil then
			Success = false
			Log(0, "Missing dependency properties")
			break
		end
		
		local InstallDir = UserConfig.GetDependencyInstallDir(self, Properties)
		
		if InstallDir == nil then
			InstallDir = "Please fill this field."
		end
		
		local Table = { Name = v, Kind = DependencyType, Properties = { Directory = InstallDir} }
		table.insert(ConfigFileBody, Table)
	end
	
	if Success == true then
		local EncodedConfigFileBody, Error = json.encode_pretty(ConfigFileBody)
		if Error == nil then
			io.writefile("Config.json", EncodedConfigFileBody)
		end
	end

end

function UserConfig.GetDependencyNamesByType(self, DependencyTypeEnum)
	local Dir = UserConfig.GetDependencyDirByType(self, DependencyTypeEnum)
	
	local Directories = os.matchdirs(Dir.. "/*")
	local Results = {}
	
	for k,v in ipairs(Directories) do
		table.insert(Results, string.match(v, "%w+$"))
	end
	
	return Results
end

function UserConfig.GetDependencyDirByType(self, DependencyTypeEnum)
	local Dir = nil
	if DependencyTypeEnum == EDependencyType.Dependency then
		Dir = GetDependenciesDir()
	elseif DependencyTypeEnum == EDependencyType.Module then
		Dir = GetModulesDir()
	elseif DependencyTypeEnum == EDependencyType.Tool then
		Dir = GetToolsDir()
	end
	return Dir
end