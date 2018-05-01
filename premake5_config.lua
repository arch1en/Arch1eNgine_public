-- This is the only file that should be interacted with.

-- Directory where cmake executable resides.
CMakeExecutableDir = "E://_Tools//CMake//bin"

-- Directory where makefile or mingw32-make executable resides.
MakefileExecutableDir = "E://_Tools//MinGW//bin"

-- [TODO] Directory where autotools executable resides.
AutotoolsExecutableDir = ""

-- Names of dependencies that will be used in the project.
-- Mind that dependency name is also a name of a folder in the Dependencies directory.
DependencyNames = { "ASSIMP",
                    "FreeType",
                    "GLEW",
                    "GLM",
                    "IMGUI",
                    "SDL2" }

-- Names of link files to use during linkage phase.
-- These values must match its index with DependencyNames.
DependencyLinkFileNames = { "assimp-vc140-mt",
                            "",
                            "glew32",
                            "",
                            "",
                            {"SDL2", "SDL2main"} }

-- Directory where include files resids.
-- These values must match its index with DependencyNames.
-- If header files are in the root directory of a dependency, then use empty string ("")
DependencyIncludeDirs = { "include",
                          "include",
                          "include",
                          "",
                          "",
                          "", }

-- Directory where linking files resids.
-- These values must match its index with DependencyNames.
-- If theres no libraries, use empty string ("")
DependencyLinkDirs = {

}
