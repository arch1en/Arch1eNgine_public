-- This is the only file that should be interacted with.

-- Directory where cmake executable resides.
CMakeExecutableDir = "E://_Tools//CMake//bin"

-- Directory where makefile or mingw32-make executable resides.
MakefileExecutableDir = "E://_Tools//MinGW//bin"

-- [TODO] Directory where autotools executable resides.
AutotoolsExecutableDir = ""


-- Example :

-- Names of dependencies that will be used in the project.
-- Mind that dependency name is also a name of a folder in the Dependencies directory.
--["Name"] =                  "Library",

-- Names of link files to use during linkage phase.
-- These values must match its index with DependencyNames.
--["LinkFileNames"] =         { "library" },

-- Directory where include files resides.
-- These values must match its index with DependencyNames.
-- If header files are in the root directory of a dependency, then use empty string ("")
--["IncludeDirs"] =           { "include" },

--["RequiresGeneration"] =    true,
--["RequiresBuilding"] =      true

Dependencies =
{
  {
    ["Name"] =                  "ASSIMP",
    ["LinkFileNames"] =         { "assimp-vc140-mt" },
    ["IncludeDirs"] =           { "include" },
    ["RequiresGeneration"] =    true,
    ["RequiresBuilding"] =      true
  },
  {
    ["Name"] =                  "FreeType",
    ["LinkFileNames"] =         {},
    ["IncludeDirs"] =           { "include" },
    ["RequiresGeneration"] =    false,
    ["RequiresBuilding"] =      false
  },
  {
    ["Name"] =                  "GLAD",
    ["LinkFileNames"] =         {},
    ["IncludeDirs"] =           { "include" },
    ["RequiresGeneration"] =    false,
    ["RequiresBuilding"] =      false
  },
  {
    ["Name"] =                  "GLM",
    ["LinkFileNames"] =         {},
    ["IncludeDirs"] =           { "" },
    ["RequiresGeneration"] =    false,
    ["RequiresBuilding"] =      false
  },
  {
    ["Name"] =                  "IMGUI",
    ["LinkFileNames"] =         {},
    ["IncludeDirs"] =           { "" },
    ["RequiresGeneration"] =    false,
    ["RequiresBuilding"] =      false,
  },
  {
    ["Name"] =                  "SDL2",
    ["LinkFileNames"] =         { "SDL2", "SDL2main" },
    ["IncludeDirs"] =           { "include" },
    ["RequiresGeneration"] =    true,
    ["RequiresBuilding"] =      true
  }
}
