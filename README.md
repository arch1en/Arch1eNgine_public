# Arch1eNgine

### Description



|	Name	|	Explanation	|
|	-----	|	-----	|
|	Author	|	Artur "Arch1eN" Ostrowski	|
|	Version	|	0.0.1 Pre-Alpha |
|	Tested Platforms |	Windows |
|	Target Platforms	|	As many as possible	|
|	Tested Compilers |	MSVC	|
|	Target Compilers	| As many as possible |

### Required tools :

|	Name	|	Link	|
|	-----	|	-----	|
|	Premake5	|	https://premake.github.io/download.html	|
|	CMake	|	https://cmake.org/	|

### Naming Convention

|	Name	|	Explanation	|
|	-----	|	-----	|
|	Foreign Dependency	|	3rd party library	|
|	Module Dependency	|	Everything that is inside of the "Module" folder |

### Build System

Build system is based on Premake5. It is capable of generating, building and organizing libraries of a foreign dependency, therefore, since these dependencies need other build tools, such as CMake or GNU Make, all of the tools are nescessary for the proper functioning of the engine. 
Apart from the standard option and action set of premake5, there are also custom made options and actions, for the convenience :

|	Command	|	Type	|	Description	|	
|	-----	|	-----	|	-----	|
|	clean	|	Action	|	Cleans the entire foreign dependency, by removing every file from its directory, except Source fodler and Properties.lua file  	|
|	generate	|	Action	|	Generates the foreign dependency according to the Properties.lua file. All generated files are landing to the Generated directory.	|
|	build	|	Action	|	Builds the foreign dependency according to the Properties.lua file.	|
|	organize	|	Action	|	 Moves built files to the Build and Library folders respectively.		|
|	configuration	|	Option	|	 Which type of configuration should the dependency be built on. Default = "all"		|
|	platform	|	Option	|	 Which type of platform should the dependency be built on. Default = "all"	|
|	dependency	|	Action	|	 Which dependency should be taken into action. Default = "all"		|
