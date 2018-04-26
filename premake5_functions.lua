-- CMake Functions
function cmake_generate(BuildDir, DependencyDir)
  --local file = assert(io.popen("cmake -E chdir " .. BuildDir .. " cmake " .. DependencyDir, 'r'))
  local file = assert(io.popen("cd /d " .. BuildDir .. " && cmake -G \"Visual Studio 15 2017 Win64\" " .. DependencyDir, 'r'))
  local output = file:read('*all')
  file:close()
  print(output)
end

function cmake_build(BuildDir)
  local file = assert(io.popen("cmake --build " .. BuildDir, 'r'))
  local output = file:read('*all')
  file:close()
  print(output)
end

-- GNU Makefile Functions
function makefile_generate(BuildDir)
  io.popen("make -C" .. BuildDir)
end

function makefile_build()

end

function generate_all()
  generate_assimp()
  generate_devil()
  generate_freetype()
  generate_glew()
  generate_sdl2()
end

function build_all()
  build_assimp()
  build_devil()
  build_freetype()
  build_glew()
  build_sdl2()
end

function clean_all()
  clean_assimp()
  clean_devil()
  clean_freetype()
  clean_glew()
  clean_sdl2()
end

-- ASSIMP Functions

function generate_assimp()
  print("Generate : Generating ASSIMP")
  cmake_generate(BuildDirs["ASSIMP"], DependencyDirs["ASSIMP"])
end

function build_assimp()
  print("Build : Building ASSIMP")
  cmake_build(DependencyDirs["ASSIMP"])
end

function clean_assimp()
  print("Clean : Cleaning ASSIMP")
  os.rmdir(BuildDirs["ASSIMP"])
end

-- DevIL Functions

function generate_devil()
  print("Generate : Generating DevIL")
  cmake_generate(BuildDirs["DevIL"], DependencyDirs["DevIL"])
end

function build_devil()
  print("Build : Building DevIL")
  cmake_build(DependencyDirs["DevIL"])
end

function clean_devil()
  print("Clean : Cleaning DevIL")
  os.rmdir(BuildDirs["DevIL"])
end

-- FreeType Functions

function generate_freetype()
  print("Generate : Generating FreeType")
  cmake_generate(BuildDirs["FreeType"], DependencyDirs["FreeType"])
end

function build_freetype()
  print("Build : Building FreeType")
  cmake_build(DependencyDirs["FreeType"])
end

function clean_freetype()
  print("Clean : Cleaning FreeType")
  os.rmdir(BuildDirs["FreeType"])
end

-- GLEW Functions

function generate_glew()
  print("Generate : Generating GLEW")
  cmake_generate(BuildDirs["GLEW"], DependencyDirs["GLEW"])
end

function build_glew(BuildDir)
  print("Build : Building GLEW")
  if os.target() == "windows" then
    os.mkdir(BuildDir .. "/lib")
    os.mkdir(BuildDir .. "/bin")

    io.popen("gcc -DGLEW_NO_GLU -O2 -Wall -W Iinclude -DGLEW_BUILD -o src/glew.o -c src/glew.c; " .. BuildDir)
    io.popen("gcc -shared -Wl,-soname,libglew32.dll -Wl, --out-implib,lib/libglew32.dll.a -o lib/glew32.dll src/glew.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32; " .. BuildDir)
    io.popen("ar cr lib/libglew32.a src/glew.o; " .. BuildDir)

    io.popen("gcc -DGLEW_NO_GLU -DGLEW_MX -O2 -Wall -W -Iinclude -DGLEW_BUILD -o src/glew.mx.o -c src/glew.c; " .. BuildDir)
    io.popen("gcc -shared -Wl,-soname,libglew32mx.dll -Wl, --out-implib,lib/libglew32mx.dll.a -o lib/glew32mx.dll src/glew.mx.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32; " .. BuildDir)
    io.popen("ar cr lib/libglew32mx.a src/glew.mx.o")
  end
  --cmake_build(DependencyDirs["GLEW"])
end

function clean_glew()
  print("Clean : Cleaning GLEW")
  os.rmdir(BuildDirs["GLEW"])
end

-- SDL2 Functions

function generate_sdl2()
  print("Generate : Generating SDL2")
  cmake_generate(BuildDirs["SDL2"], DependencyDirs["SDL2"])
end

function build_sdl2()
  print("Build : Building SDL2")
  cmake_build(DependencyDirs["SDL2"])
end

function clean_sdl2()
  print("Clean : Cleaning SDL2")
  os.rmdir(BuildDirs["SDL2"])
end
