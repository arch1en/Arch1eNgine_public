
function AdaptedDirSlash()
    if os.target() == "windows" then
        return "\\"
    elseif os.target() == "linux"  then
        return "/"
    end

     return "/"
end

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

function GetPropertiesFromFile(Path)
	local Result = dofileopt(Path)
    if Result ~= nil then
        return Properties
    end
end

function GetPropertiesFileName()
	return "Properties"
end

function GetGeneratedFolderName()
	return "Generated"
end	

function GetApplicationFolderName()
	return "Application"
end	

function GetSourceFolderName()
	return "Source"
end	

function GetBuildFolderName()
	return "Build"
end	

function GetBinariesFolderName()
	return "Binaries"
end	

function GetToolsFolderName()
	return "Tools"
end	

function GetLibrariesFolderName()
	return "Libraries"
end

function GetDependenciesFolderName()
	return "Dependencies"
end

function GetModulesFolderName()
	return "Modules"
end

function GetApplicationDir()
	return AdaptDirSlashes(GetWorkspaceDir().. "/" ..ApplicationFolderName)
end

function GetBinariesDir()
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..BinariesFolderName)
end

function GetWorkspaceDir()
	return AdaptDirSlashes(io.popen("cd"):read('*l'))
end

function GetDependenciesDir()
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetDependenciesFolderName())
end

function GetModulesDir()
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetModulesFolderName())
end

function GetToolsDir()
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetToolsFolderName())
end

function GetDependencyDir(DependencyName)
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetDependenciesFolderName().. "/" ..DependencyName)
end

function GetDependencySourceDir(DependencyName)
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetDependenciesFolderName().. "/" ..DependencyName.. "/" ..GetSourceFolderName())
end

function GetDependencyGeneratedDir(DependencyName)
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetDependenciesFolderName().. "/" ..DependencyName.. "/" ..GetGeneratedFolderName())
end

function GetDependencyBuildDir(DependencyName)
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetDependenciesFolderName().. "/" ..DependencyName.. "/" ..GetBuildFolderName())
end

function GetDependencyLibrariesDir(DependencyName)
    return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetDependenciesFolderName().. "/" ..DependencyName.. "/" ..GetLibrariesFolderName())
end

function GetModuleDependencyLibrariesDir(ModuleName)
	return AdaptDirSlashes(GetWorkspaceDir().. "/" ..GetModulesFolderName().. "/" ..ModuleName.. "/" ..GetLibrariesFolderName())
end