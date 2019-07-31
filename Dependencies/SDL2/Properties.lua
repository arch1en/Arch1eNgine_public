Properties =
{
    Name =                  "SDL2",
    IncludeDirs =           { "Source/include" },
    RequiresGeneration =    true,
    RequiresBuilding =      true,
	LinkageType	=			"Dynamic",
    ConfigurationProperties =
    { 
        {
            Name = "Release",
			Mapping = "Release"
        },
        { 
            Name = "Debug",
			Mapping = "Debug"
        }
    },
    PlatformProperties = -- Relative to this project platform name (x64).
    {
        {
            Name = "Win32",
			Mapping = "Win32"
        },
        {
            Name = "x64",
            Mapping = "Win64"
        }
    },
    PropertyGroups =
    {
        {
            Name = "Release|Win32",
            LinkFileNames = { "SDL2", "SDL2main" }
        },
        {
            Name = "Debug|Win32",
            LinkFileNames = { "SDL2d", "SDL2maind" }
        },
        {
            Name = "Release|x64",
            LinkFileNames = { "SDL2", "SDL2main" }
        },
        {
            Name = "Debug|x64",
            LinkFileNames = { "SDL2d", "SDL2maind" }
        }
    }
}

