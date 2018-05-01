-- CMake Functions
-- Reads cmd output one line by one.
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

function LiveLog(log)
  repeat
    local logLine = log:read(1)
    if logLine then
      io.write(logLine) io.flush()
    end
  until not logLine
  log:close()
end

function FolderExists(Path)
  local FileHandle, StrError = io.open(Path.."\\*.*","r")
  if FileHandle ~= nil then
    io.close(FileHandle)
    return true
  else
    if string.match(StrError, "No such file or directory") then
      return false
    else
      return true
    end
  end
end

function CreateFolderIfDoesntExist(DependencyName)
  if FolderExists(BuildDirs[DependencyName]) == false then
    os.mkdir(BuildDirs[DependencyName])
    return DependencyName .. " Build folder missing. Creating..."
  else
    return DependencyName .. " Build folder exists. Onward..."
  end
end

-- Returns directory where file resids, otherwise returns false.
function GetFilePath(InitialDirectory, FileName, Recursive)
  local command = ""
  if os.target() == "windows" then
    InitialDirectory = AdaptDirSlashes(InitialDirectory)
    command = "dir " ..InitialDirectory.. " /b"
  end

  local result = io.popen(command)
  for filename in result:lines() do
    print(filename:match('%w+%.1%w+'))
    if Recursive == true then

    end
    --print(filename)
  end
  result:close()


end

function cmake_generate(BuildDir, DependencyDir)
  local log = io.popen("cd /d " .. BuildDir .. " && " ..CMakeAbsoluteDir.. "//cmake -G \"Visual Studio 15 2017 Win64\" " .. DependencyDir, 'r')
  LiveLog(log)
end

function cmake_build(BuildDir)
  local log = io.popen("cd /d " .. BuildDir .. " && cmake --build .", 'r')
  LiveLog(log)
end

-- GNU Makefile Functions
function makefile_generate(BuildDir)
  local log = io.popen("make -C" .. BuildDir)
  LiveLog(log)
end

function makefile_build()

end

function generate_all()
  generate_assimp()
  --generate_devil()
  generate_freetype()
  generate_glew()
  generate_sdl2()
end

function build_all()
  build_assimp()
  --build_devil()
  build_freetype()
  build_glew()
  build_sdl2()
end

function clean_all()
  clean_assimp()
  --clean_devil()
  clean_freetype()
  clean_glew()
  clean_sdl2()
end

-- ASSIMP Functions

function generate_assimp()
  print("Generate : Generating ASSIMP")
  print("Generate : " ..CreateFolderIfDoesntExist("ASSIMP"))
  cmake_generate(BuildDirs["ASSIMP"], DependencyDirs["ASSIMP"])
end

function build_assimp()
  print("Build : Building ASSIMP")
  cmake_build(BuildDirs["ASSIMP"])
end

function clean_assimp()
  print("Clean : Cleaning ASSIMP")
  os.rmdir(BuildDirs["ASSIMP"])
end

-- DevIL Functions
-- DevIL was replaced by stb_image.

--function generate_devil()
--  print("Generate : Generating DevIL")
--  print("Generate : " ..CreateFolderIfDoesntExist("DevIL"))
--  cmake_generate(BuildDirs["DevIL"], DependencyDirs["DevIL"])
--end

--function build_devil()
--  print("Build : Building DevIL")
--  cmake_build(BuildDirs["DevIL"])
--end

--function clean_devil()
--  print("Clean : Cleaning DevIL")
--  os.rmdir(BuildDirs["DevIL"])
--end

-- FreeType Functions

function generate_freetype()
  print("Generate : Generating FreeType")
  print("Generate : " ..CreateFolderIfDoesntExist("FreeType"))
  cmake_generate(BuildDirs["FreeType"], DependencyDirs["FreeType"])
end

function build_freetype()
  print("Build : Building FreeType")
  cmake_build(BuildDirs["FreeType"])
end

function clean_freetype()
  print("Clean : Cleaning FreeType")
  os.rmdir(BuildDirs["FreeType"])
end

-- GLEW Functions

function generate_glew()
  print("Generate : Generating GLEW")
  print("Generate : " ..CreateFolderIfDoesntExist("GLEW"))
  makefile_generate(BuildDirs["GLEW"])
  --cmake_generate(BuildDirs["GLEW"], DependencyDirs["GLEW"])
end

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

function clean_glew()
  print("Clean : Cleaning GLEW")
  os.rmdir(BuildDirs["GLEW"])
end

-- SDL2 Functions

function generate_sdl2()
  print("Generate : Generating SDL2")
  print("Generate : " ..CreateFolderIfDoesntExist("SDL2"))
  cmake_generate(BuildDirs["SDL2"], DependencyDirs["SDL2"])
end

function build_sdl2()
  print("Build : Building SDL2")
  cmake_build(BuildDirs["SDL2"])
end

function clean_sdl2()
  print("Clean : Cleaning SDL2")
  os.rmdir(BuildDirs["SDL2"])
end
