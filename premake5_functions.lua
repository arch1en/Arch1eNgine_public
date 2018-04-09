-- CMake Functions
function cmake_generate(BuildDir, DependencyDir)
  io.popen("cmake -E chdir " .. BuildDir .. "cmake " .. DependencyDir)
end

function cmake_build(BuildDir)
  io.popen("cmake --build " .. BuildDir)
end

-- GNU Makefile Functions
function makefile_generate(BuildDir)
  io.popen("make -C" .. BuildDir)
end

function makefile_build()

end

-- ASSIMP Functions

function generate_assimp()
  cmake_generate(BuildDirs["ASSIMP"], DependencyDirs["ASSIMP"])
end

function build_assimp()
  cmake_build(DependencyDirs["ASSIMP"])
end

function clean_assimp()
  os.rmdir(BuildDirs["ASSIMP"])
end

-- DevIL Functions

function generate_devil()
  cmake_generate(BuildDirs["DevIL"], DependencyDirs["DevIL"])
end

function build_devil()
  cmake_build(DependencyDirs["DevIL"])
end

function clean_devil()
  os.rmdir(BuildDirs["DevIL"])
end

-- FreeType Functions

function generate_freetype()
  cmake_generate(BuildDirs["FreeType"], DependencyDirs["FreeType"])
end

function build_freetype()
  cmake_build(DependencyDirs["FreeType"])
end

function clean_freetype()
  os.rmdir(BuildDirs["FreeType"])
end

-- GLEW Functions

function generate_glew()
  cmake_generate(BuildDirs["GLEW"], DependencyDirs["GLEW"])
end

function build_glew()
  cmake_build(DependencyDirs["GLEW"])
end

function clean_glew()
  os.rmdir(BuildDirs["GLEW"])
end

-- SDL2 Functions

function generate_sdl2()
  cmake_generate(BuildDirs["SDL2"], DependencyDirs["SDL2"])
end

function build_sdl2()
  cmake_build(DependencyDirs["SDL2"])
end

function clean_sdl2()
  os.rmdir(BuildDirs["SDL2"])
end
