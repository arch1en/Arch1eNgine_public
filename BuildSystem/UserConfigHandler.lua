
require("GlobalFunctions")

UserConfigHandler = {
	ConfigFileData = {}
}

ToolField = 
{
	
}

function UserConfigHandler.New(self, o)
	o = o or {}
	local self = setmetatable(o, UserConfigHandler)
	self.__index = o
	local ErrorMessage = ""
	self.ConfigFileData, ErrorMessage = UserConfigHandler.LoadConfigData(self)
	if ErrorMessage then
		Log(0, ErrorMessage)
	end
	
	return o
end

function UserConfigHandler.FindDepedencyDataByName(self, Name)
	for _,v in ipairs(self.ConfigFileData) do
		if v.Name == Name then
			return v
		end
	end
	
	return nil
end

--[[
	ErrorCodes : 
	1 (Warning) - Dependency doesn't have specified PathData.
]]--
function UserConfigHandler.GetDependencyInstallDir(self, Properties)

	local ErrorCode = 0
	local PathData = nil
	
	if Properties.UserConfig.PathData == nil then
		Log(1, "\""..Properties.Name .."\" dependency doesn't have specified PathData. Omitting...")
		ErrorCode = 1
		return nil, ErrorCode
	end	
	
	if os.target() == "windows" then
		PathData = Properties.UserConfig.PathData.Windows
	end
		
	local Path = UserConfigHandler.GetRegistryValue(self, PathData.Registry)

	if Path == nil or Path == "" then
		
		for k,v in ipairs(PathData.Defaults) do
			
			local MatchResult = os.matchdirs(AdaptDirSlashes(v))
				
			if MatchResult[1] ~= nil then
				
				return MatchResult[1], ErrorCode
					
			end
		end
	end
	
	return Path, ErrorCode
end

function UserConfigHandler.GetDependencyProperties(self, DependencyTypeEnum, Name)
	local DependenciesDir = UserConfigHandler.GetDependencyDirByType(self, DependencyTypeEnum)
	local Path = os.matchfiles(AdaptDirSlashes(DependenciesDir.."/"..Name.."/"..GetPropertiesFileName()..".lua"))
	if Path ~= nil and Path[1] ~= nil then
		return GetPropertiesFromFile(Path[1])
	end
end

function UserConfigHandler.GetRegistryValue(self, RegistryPath)
	local RegistryResult = ""
	
	if os.target() == "windows" then
		RegistryResult = os.getWindowsRegistry(RegistryPath)
	end
	
	return RegistryResult
end

function UserConfigHandler.LoadConfigData(self)
	local ConfigFileData = io.readfile("Config.json")
	return json.decode(ConfigFileData)
end

function UserConfigHandler.GenerateUserConfigFile(self)

	local ConfigFileBody = {}
	
	for _,v0 in pairs(EDependencyType) do

		local DependencyType = v0
		local Names = UserConfigHandler.GetDependencyNamesByType(self, DependencyType)
	
		for _,v in ipairs(Names) do
			
			local Properties = UserConfigHandler.GetDependencyProperties(self, DependencyType, v)
			local UserConfig = {}
			
			if Properties ~= nil then
				UserConfig = Properties.UserConfig
				
				if UserConfig ~= nil then
					local InstallDir, ErrorCode = UserConfigHandler.GetDependencyInstallDir(self, Properties)
		
					if ErrorCode == 0 then
						if InstallDir == nil then
							InstallDir = "Please fill this field."
						end
		
						local Table = { Name = v, Kind = DependencyType, Properties = { Directory = InstallDir } }
						if UserConfig.AdditionalData then
							for k,v in pairs(UserConfig.AdditionalData) do
								Table.Properties[k] = v
							end
						end
						table.insert(ConfigFileBody, Table)
					else
						Success = false
					end
				else
					Log(1, "Warning : Missing UserConfig data.")
				end
			else
				Log(0, "Warning : Missing \""..v.."\" dependency properties.")
			end
		end
	end

	if TableLength(ConfigFileBody) > 0 then
		local EncodedConfigFileBody, Error = json.encode_pretty(ConfigFileBody)
		
		if Error == nil then
			io.writefile("Config.json", EncodedConfigFileBody)
		end
	else
		Log(0, "User config generation failed.")
	end

end

function UserConfigHandler.GetDependencyNamesByType(self, DependencyTypeEnum)
	local Dir = UserConfigHandler.GetDependencyDirByType(self, DependencyTypeEnum)
	
	local Directories = os.matchdirs(Dir.. "/*")
	local Results = {}
	
	for k,v in ipairs(Directories) do
		local Match = string.match(v, "[a-zA-Z0-9%_%-]+$")
		table.insert(Results, Match)
	end
	
	return Results
end

function UserConfigHandler.GetDependencyDirByType(self, DependencyTypeEnum)
	local Dir = nil
	if DependencyTypeEnum == EDependencyType.Foreign then
		Dir = GetDependenciesDir()
	elseif DependencyTypeEnum == EDependencyType.Module then
		Dir = GetModulesDir()
	elseif DependencyTypeEnum == EDependencyType.Tool then
		Dir = GetToolsDir()
	end
	return Dir
end
