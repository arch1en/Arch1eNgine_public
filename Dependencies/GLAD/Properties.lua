Properties = 
{
    Name =                  "GLAD",
    LinkFileNames =         {},
    IncludeDirs =           { "Source/include", "Source/src" },
    RequiresGeneration =    false,
    RequiresBuilding =      false,
    PostPremakeCommand = function()
        local RequiredGladFilePath = AdaptDirSlashes(GetDependencySourceDir("GLAD").. "/src/glad.c") 
        files(RequiredGladFilePath)
        filter("files:glad.c")
        flags("NoPCH")
    end
}
