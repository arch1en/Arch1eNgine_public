-- This is the only file that should be interacted with.

-- Directory where cmake executable resides.
CMakeExecutableDir = "E://_Tools//CMake//bin"

-- Directory where makefile or mingw32-make executable resides.
MakefileExecutableDir = "E://_Tools//MinGW//bin"

-- [TODO] Directory where autotools executable resides.
AutotoolsExecutableDir = ""

--[[
  Example :

  Names of dependencies that will be used in the project.
  Mind that dependency name is also a name of a folder in the Dependencies directory.
  ["Name"] =                  "Library",

  Names of link files to use during linkage phase.
  These values must match its index with DependencyNames.
  ["LinkFileNames"] =         { "library" },

  Directory where include files resides.
  These values must match its index with DependencyNames.
  If header files are in the root directory of a dependency, then use empty string ("")
  ["IncludeDirs"] =           { "include" },

  If true, will generate files that will reside inside of "Builds/Dependencies" folder, otherwise it will skip generation.
  ["RequiresGeneration"] =    true,

  If generation is required, these properties will be used for further action.
  ["GenerationProperties"]
    ToolType = "auto" / "buildsystem" / "compiler"
      auto - automatically determines which tool should be used.
      buildsystem - uses build system to generate dependencies.
    Tool = "auto" / cmake" / "makefile" / "visualstudio" / "mingw" / "cygwin"
      auto - 
      cmake - uses cmake as a generation tool. Only evaluated if "auto" or "buildsystem" ToolType is picked.
      makefile - uses makefile as a generation tool. Only evaluated if "auto" or "buildsystem" ToolType is picked.
  If true, will build the dependency with BuildDependency function, otherwise doesn't do so. Built libraries will be automatically moved to the respective catalogues with use of "organize" action.
  ["RequiresBuilding"] =      true

  If building is required, these properties will be used for further action.
  ["BuildingProperties"]
--]]

Configurations =
{
  {
    ["Name"] = "Release",
    ["Defines"] = {},
    ["Symbols"] = "Off",
    ["Optimize"] = "On"
  },
  {
    ["Name"] = "Debug",
    ["Defines"] = { "DEBUG" },
    ["Symbols"] = "On",
    ["Optimize"] = "Off"
  }
}

Dependencies =
{
  {
    ["Name"] =                    "ASSIMP",
    ["LinkFileNames"] =           { "assimp-vc140-mt" },
    ["IncludeDirs"] =             { "include" },
    ["RequiresGeneration"] =      true,
    ["RequiresBuilding"] =        true
  },
  {
    ["Name"] =                  "FreeType",
    ["LinkFileNames"] =         {},
    ["IncludeDirs"] =           { "include" },
    ["RequiresGeneration"] =    false,
    ["RequiresBuilding"] =      false,

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
    ["RequiresBuilding"] =      true,
    ["BuildingProperties"] =
    { 
        "Configurations" = { "Debug", "Release" }, 
    },
  }
}

-- TEST
print(Dependencies[6]["BuildingProperties"]["Configurations"][0])
-- ~TEST~