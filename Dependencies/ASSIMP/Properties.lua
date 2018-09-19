Properties =  
{
    ["Name"] =                    "ASSIMP",
    ["IncludeDirs"] =             { "Source/include", "Generated/include" },
    ["RequiresGeneration"] =      true,
    ["RequiresBuilding"] =        true,
    ["ConfigurationProperties"] =
    {
        {
            ["Name"] = "Release",    
        },
        {
            ["Name"] = "Debug",
        }
    },
    ["PlatformProperties"] =
    {
        {
            ["Name"] = "Win32",
        },
    },
    ["PropertyGroups"] =
    {
        {
            ["Name"] = "Debug|Win32",
            ["LinkFileNames"] = { "assimp-vc140-mt" }
        }
    },
    ["CustomBuildCommand"] = function()
        includedirs(GetDependencyGeneratedDir("ASSIMP"))
    end
}
