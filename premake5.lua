require("BuildSystem/BuildConfiguration")
require("BuildSystem/LogSystem")
require("BuildSystem/BuildFunctions")
require("BuildSystem/BuildActions")
require("BuildSystem/BuildOptions")
require("BuildSystem/Project_Application")
require("BuildSystem/Project_Tests")

--require("BuildSystem/FileFinder")

workspace("Arch1eNgine")

SetupApplicationProject()
SetupTestsProject()
