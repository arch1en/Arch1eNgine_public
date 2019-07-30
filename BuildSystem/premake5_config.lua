 

--[[

Disclaimer :
	This is the only file that should be interacted with.

	
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
		
		Define configuration names that will be used during building process. 
		For the proper build behavior, these options mave to match project configurations index.
		["Configurations"] = { "Release", "Debug" }

--]]

-- Directory where cmake executable resides.
CMakeExecutableDir = "D:/_Tools/CMake/bin"

-- Directory where makefile or mingw32-make executable resides.
MakefileExecutableDir = "D:/_Tools/MinGW/bin"

-- [TODO] Directory where autotools executable resides.
AutotoolsExecutableDir = ""

Configurations =
{
  {
    Name = "Release",
    Defines = {},
    Symbols = "Off",
    Optimize = "On"
  },
  {
    Name = "Debug",
    Defines = { "DEBUG" },
    Symbols = "On",
    Optimize = "Off"
  }
}

Platforms =
{
    {
        Name = "Win32",
        Defines = { "PLATFORM_WIN32" },
		System = "Windows",
		Architecture = "x32"
    },
    {
        Name = "Win64",
        Defines = { "PLATFORM_WIN64" },
		System = "Windows",
		Architecture = "x64"
    },
    {
        Name = "Linux",
        Defines = { "PLATFORM_LINUX" },
		System = "Linux",
		Architecture = "x64"
    }
}

Compilers =
{
	MSVC = {
		Name = "MSVC",
		Toolset = "v120"
	}
}
