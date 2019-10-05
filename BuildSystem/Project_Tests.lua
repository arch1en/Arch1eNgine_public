
local ProjectName = "Tests";

function SetupTestsProject()

    filter{}

    -- Configurations
    ConfigurationNames = {}
    for i,c in pairs(Configurations) do
        ConfigurationNames[i] = c.Name
    end
    configurations(ConfigurationNames)
    
    -- Platforms
    PlatformNames = {}
    for j,p in pairs(Platforms) do
          PlatformNames[j] = p.Name
    end
    platforms(PlatformNames)
    
    location(ProjectName)

    project(ProjectName)
    kind("ConsoleApp")
    language("C++")
    cppdialect("C++17")
    targetdir(GetBinariesDir().."/%{cfg.buildcfg}/%{cfg.platform}")
    location("Application/%{prj.name}")

    -- Adding source files to the project...
    files({"%{prj.name}/Source/**.h", "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.inl"})
    -- use removefiles function to remove any unnescessary files.

    -- Including directories...
    
    local ApplicationProperties = GetApplicationProperties()
    if ApplicationProperties == nil then
        Log(0,"Error configuring application. Properties.lua not found !")
        return 
    end

	SetupDependencies("Module", ApplicationProperties)
	SetupDependencies("Foreign", ApplicationProperties)
	
	defines{"MODULE_API=__declspec(dllimport)"}
	
    filter("system:windows")
    links({"OpenGL32"})

    filter("system:not windows")
    links({"GL"})

    filter {}

    -- Adding configuration defines
    
    for i,v in pairs(Configurations) do

        filter("configurations:" .. v.Name)
        if #v.Defines ~= 0 then
            local DefinesNames = {}
            for j,w in pairs(v.Defines) do
                DefinesNames[j] = w
            end
            defines(DefinesNames)
        end

        symbols(v.Symbols)
        optimize(v.Optimize)

    end

    filter{}

    for _,v in pairs(Platforms) do
        if #v.Defines ~= 0 then
            filter{"platforms:"..v.Name}
            local NewDefines = {}
            for i,w in pairs(v.Defines) do
                NewDefines[i] = w
            end
            defines(NewDefines)
			system(v.System)
		    architecture(v.Architecture)
        end
    end

    filter{}

    filter{"action:vs*"}
    systemversion(os.winSdkVersion() .. ".0")

    filter{}

end
