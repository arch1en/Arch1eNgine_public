require("BuildSystem/premake5_config")
require("BuildSystem/premake5_log")
require("BuildSystem/premake5_functions")
require("BuildSystem/premake5_actions")
require("BuildSystem/premake5_options")

--require("BuildSystem/FileFinder")

workspace("Arch1eNgine")

SetupApplication()

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
