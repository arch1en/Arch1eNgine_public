-- CMake Functions

function AdaptDirSlashes(String)
  if os.istarget("windows") then
    return String:gsub("/","\\")
  elseif os.istarget("linux") then
    return String:gsub("\\","/")
  else
    return String:gsub("\\","/")
  end
  return String
end

-- Initialization block
BinariesDir = "Binaries"
DependenciesDir = "Dependencies"
BuildsDir = "Builds"
LibrariesDir = "Libraries"
DependencyBuildsDir = BuildsDir.. "/" ..DependenciesDir
DependencyLibrariesDir = BuildsDir.. "/" ..LibrariesDir.. "/" ..DependenciesDir

WorkspaceDirectory = AdaptDirSlashes(io.popen("cd"):read('*l')) -- ugly hack, because premake tokens doesnt work -.-

DependencyDirs = {}

for _,v in ipairs(Dependencies) do
  DependencyDirs[v["Name"]] = AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependenciesDir.. "/" ..v["Name"])
end

DependenciesBuildDirs = {}

for _,v in pairs(Dependencies) do
  DependenciesBuildDirs[v["Name"]] = AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependencyBuildsDir.. "/" ..v["Name"])
end

for _,v in ipairs(Dependencies) do
  for _,w in pairs(v["IncludeDirs"]) do
    w = DependencyDirs[v["Name"]].. "/" ..w
  end
end

--for _,v in ipairs(Dependencies) do
--  for _,w in pairs(v["LinkDirs"]) do
--    w = DependencyDirs[v["Name"]].. "/" ..w
--  end
  --print(v)
--end

-- ~ Initialization block

-- Reads cmd output one line by one.
--- Check if a file or directory exists in this path
--function FileExists(Path)
--   local ok, err, code = os.rename(file, file)
--   if not ok then
--      if code == 13 then
         -- Permission denied, but it exists
--         return true
--      end
--   end
--   return ok, err
--end

--- Check if a directory exists in this path
--function IsDir(Path)
   -- "/" works on both Unix and Windows
--   return FileExists(AdaptDirSlashes(Path.."/"))
--end

-- os.copyfile doesnt work for windows for some reason...
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
      if v["Name"]:lower() == DependencyName:lower() then
        return i
      end
    else
      if v["Name"] == DependencyName then
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

function FindDependencyLibraryFiles(DependencyIndex)
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

  for i,v in pairs(Dependencies[DependencyIndex]["LinkFileNames"]) do
    local FileName = v.. "." ..DynamicLibraryFileExtension
    local FoundFiles = os.matchfiles(DependenciesBuildDirs[Dependencies[DependencyIndex]["Name"]].. "/**" ..FileName)
    if FoundFiles[1] == nil then
      print("FindDependencyLibraryFiles Failed. "..FileName.." link file name was not found.")
      return nil
    end
    FilePaths[1][i] = FoundFiles[1]
    local LinkerLibraryFile = FoundFiles[1]:gsub("(" ..DynamicLibraryFileExtension..")$", LinkerLibraryFileExtension)
    local LinkerLibraryFound = os.isfile(LinkerLibraryFile)
    if LinkerLibraryFound == true then
      FilePaths[2][i] = LinkerLibraryFile
    else
      printf("FindDependencyLibraryFiles Failed. " ..LinkerLibraryFile.. " file not found.")
    end
  end

  return FilePaths
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

--function CreateDependencyBuildFolderIfDoesntExist(DependencyName)
--  if os.isdir(DependenciesBuildDirs[DependencyName]) == false then
--    os.mkdir(DependenciesBuildDirs[DependencyName])
--    return DependencyName .. " Build folder missing. Creating..."
--  else
--    return DependencyName .. " Build folder exists. Onward..."
--  end
--end

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

function CMakeGenerate(BuildDir, DependencyDir)
  local log = ""
  if os.target() == "windows" then
    log = io.popen("cd /d " .. BuildDir .. " && " ..AdaptDirSlashes(CMakeExecutableDir.. "//cmake").. " -G \"Visual Studio 15 2017 Win64\" " .. DependencyDir, 'r')
  else
    print("CMakeGenerate Error : Operational System target invalid or inoperable.")
    return
  end
  LiveLog(log)
end

function CMakeBuild(BuildDir)
  local log = ""
  if os.target() == "windows" then
    log = io.popen("cd /d " .. BuildDir .. " && cmake --build . --config Release", 'r')
  else
    print("CMakeBuild Error : Operational System target invalid or inoperable.")
    return
  end
  LiveLog(log)
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
  for i,_ in ipairs(Dependencies) do
    CleanDependency(i)
  end
end

function GenerateAllDependencies()
  for i,_ in ipairs(Dependencies) do
    GenerateDependency(i)
  end
end

function BuildAllDependencies()
  for i,_ in ipairs(Dependencies) do
    BuildDependency(i)
  end
end

function OrganizeAllDependencies()
  for i,_ in ipairs(Dependencies) do
    OrganizeDependency(i)
  end
end

-- ASSIMP Functions

function GetDependencyNameByOption(aOption)
  for _, v in ipairs(DependencyNames) do
    if aOption == v:lower() then
      return v
    end
  end
  return ""
end

-- Returns name of the tool, that this dependency is using.
function DetermineDependencyBuildTool(DependencyName)
  if os.matchfiles(DependencyDirs[DependencyName].. "/CMakeLists.txt")[1] ~= nil then
    return "cmake"
  elseif os.matchfiles(DependencyDirs[DependencyName].. "/Makefile")[1] ~= nil then
    return "makefile"
  end
  return ""
end

function CleanDependency(DependencyIndex)
  local Dependency = Dependencies[DependencyIndex]["Name"]
  if Dependency == "" then
    print("Error : Cannot clean " ..Dependency.. " option.")
    return
  end

  if os.isdir(DependenciesBuildDirs[Dependency]) then
    print("Clean : Cleaning " ..Dependency)
    os.rmdir(DependenciesBuildDirs[Dependency])
  else
    print("Clean : Build directory for " ..Dependency.. " inaccessible or cleaned already.")
  end
end

function GenerateDependency(DependencyIndex)
  if Dependencies[DependencyIndex]["RequiresGeneration"] == false then
    print("Generate : " ..Dependencies[DependencyIndex]["Name"].. " doesn't require generation. Skipping...")
    return
  end
  local Dependency = Dependencies[DependencyIndex]["Name"]
  if Dependency == "" then
    print("Error : Cannot generate " ..Dependency.. " option.")
    return
  end

  local GenerationTool = DetermineDependencyBuildTool(Dependency)

  print(GenerationTool)
  print("Generate : Generating " ..Dependency)

  print("Generate : " ..CreateFolderIfDoesntExist(DependencyBuildsDir, Dependency))

  if GenerationTool == "cmake" then
    CMakeGenerate(DependenciesBuildDirs[Dependency], DependencyDirs[Dependency])
  elseif GenerationTool == "makefile" then
    MakefileGenerate(DependenciesBuildDirs[Dependency], DependencyDirs[Dependency])
  end
end

function BuildDependency(DependencyIndex)
  local CurrentDependency = Dependencies[DependencyIndex]
  if CurrentDependency["RequiresBuilding"] == false then
    print("Build : " ..CurrentDependency["Name"].. " doesn't require building. Skipping...")
    return
  end
  local Dependency = CurrentDependency["Name"]
  if Dependency == "" then
    print("Error : Cannot build " ..Dependency.. " option.")
    return
  end

  local BuildTool = DetermineDependencyBuildTool(Dependency)

  if BuildTool == "cmake" then
    print(CurrentDependency["BuildingProperties"]["Configurations"][0])
   
    CMakeBuild(DependenciesBuildDirs[Dependency], DependencyDirs[Dependency])
  elseif BuildTool == "make" then
    MakefileBuild(DependenciesBuildDirs[Dependency], DependencyDirs[Dependency])
  end

end

function IsOrganizeable()
  for _,v in pairs(Configurations) do
    if v["Name"] then
      return true
    end
  end
  return false
end

function OrganizeDependency(DependencyIndex)
  if #Dependencies[DependencyIndex]["LinkFileNames"] == 0 then
    print("Organize : " ..Dependencies[DependencyIndex]["Name"].. " have no files to link. Skipping...")
    return
  end
  local FoundFiles = FindDependencyLibraryFiles(DependencyIndex)
  --Move library files to the Builds/Libraries folder, and Dynamic Libraries to the Binaries folder.
  if FoundFiles == nil then
    print(Dependencies[DependencyIndex]["Name"].. "Organize : Error, no libraries found.")
    return
  end

  local Dependency = Dependencies[DependencyIndex]["Name"]
  print("Organize : " ..CreateFolderIfDoesntExist(DependencyLibrariesDir, Dependency))

  -- Copy dynamic link files (dll and a) to respective folder.
  for _,v in pairs(FoundFiles[1]) do
    for _,w in pairs(Configurations) do
      print("Organize : " ..CreateFolderIfDoesntExist(AdaptDirSlashes(WorkspaceDirectory.. "/" ..BinariesDir), w["Name"]))
      CopyFile(AdaptDirSlashes(v), AdaptDirSlashes(WorkspaceDirectory.. "/" ..BinariesDir.. "/" ..w["Name"]))
    end
    --print(os.copyfile(AdaptDirSlashes(v), AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependencyLibrariesDir.. "/" ..Dependency)))
  end
  -- Copy linker files (lib) to respective folder.
  for _,v in pairs(FoundFiles[2]) do
    CopyFile(AdaptDirSlashes(v), AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependencyLibrariesDir.. "/" ..Dependency))
    --print(os.copyfile(AdaptDirSlashes(v), AdaptDirSlashes(WorkspaceDirectory.. "/" ..DependencyLibrariesDir.. "/" ..Dependency)))
  end
end

-- GLEW Functions

function build_glew(BuildDir)
  print("Build : Building GLEW")
  --if os.target() == "windows" then
  --  os.mkdir(BuildDir .. "/lib")
  --  os.mkdir(BuildDir .. "/bin")

  --  io.popen("gcc -DGLEW_NO_GLU -O2 -Wall -W Iinclude -DGLEW_BUILD -o src/glew.o -c src/glew.c; " .. BuildDir)
  --  io.popen("gcc -shared -Wl,-soname,libglew32.dll -Wl, --out-implib,lib/libglew32.dll.a -o lib/glew32.dll src/glew.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32; " .. BuildDir)
  --  io.popen("ar cr lib/libglew32.a src/glew.o; " .. BuildDir)

  --  io.popen("gcc -DGLEW_NO_GLU -DGLEW_MX -O2 -Wall -W -Iinclude -DGLEW_BUILD -o src/glew.mx.o -c src/glew.c; " .. BuildDir)
  --  io.popen("gcc -shared -Wl,-soname,libglew32mx.dll -Wl, --out-implib,lib/libglew32mx.dll.a -o lib/glew32mx.dll src/glew.mx.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32; " .. BuildDir)
  --  io.popen("ar cr lib/libglew32mx.a src/glew.mx.o")
  --end
  --cmake_build(DependencyDirs["GLEW"])
end
