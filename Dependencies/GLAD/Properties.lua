Properties = 
{
    Name =                  "GLAD",
    LinkFileNames =         {},
    IncludeDirs =           { "Source/include" },
    RequiresGeneration =    false,
    RequiresBuilding =      false,
    PostPremakeCommand = function()
        local RequiredGladFilePath = AdaptDirSlashes(GetDependencySourceDir("GLAD").. "/src/glad.c") 
        files(RequiredGladFilePath)
        filter("files:glad.c")
        flags("NoPCH")
    end
}
