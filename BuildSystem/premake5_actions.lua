require("BuildSystem/UserConfigHandler")

premake.modules.lua = {}

local m = premake.modules.lua
local p = premake

newaction
{
  trigger = "init",
  description = "Initialize project. That includes configuration file generations.",
  onStart = function()
    Log(0,"Initiation : Starting process")
	local uch = UserConfigHandler.New()
	UserConfigHandler.GenerateUserConfigFile(uch)
  end,

  onEnd = function()
    print("Initiation : Finished")
  end,
}

newaction
{
  trigger = "clean",
  description = "Clean dependency that is currently set.",
  onStart = function()
    Log(0,"Clean : Starting process")
    if _OPTIONS["dependency"] == "all" then
      CleanAllDependencies()
    else
      CleanDependency(_OPTIONS["dependency"])
    end
  end,

  onEnd = function()
    print("Clean : Finished")
  end,
}

newaction
{
  trigger = "generate",
  description = "Generate dependency that is currently set.",
  onStart = function()
    if _OPTIONS["dependency"] == "all" then
      GenerateAllDependencies()
    else
      GenerateDependency(_OPTIONS["dependency"])
    end
  end,

  onEnd = function()
    print("Generate : Finished")
  end
}

newaction
{
  trigger = "build",
  description = "Build dependency that is currently set.",
  onStart = function()
      print("Build : Starting process")
      if _OPTIONS["dependency"] ~= nil then
          if _OPTIONS["dependency"] == "all" then
              BuildAllDependencies()
          else
              BuildDependency(_OPTIONS["dependency"])
          end
      elseif _OPTIONS["module"] ~= nil then
        if _OPTIONS["module"] == "all" then
            BuildAllModules()
        else
            BuildModule(_OPTIONS["module"])
        end
      end
  end,

  onEnd = function()
      print("Build : Finished")
  end

}

newaction
{
    trigger = "organize",
    description = "Organize libraries of the dependency that is currently set.",
    onStart = function()
        if IsOrganizeable() then
            Log(0,"Organize : Starting process")
            if _OPTIONS["dependency"] == "all" then
                OrganizeAllDependencies()
            else
                OrganizeDependency(_OPTIONS["dependency"], _OPTIONS["configuration"], _OPTIONS["platform"])
            end
        else
            print("Organize : Process failed.")
        end
    end,

    onEnd = function()
        print("Organize : Finished")
    end
}

newaction
{
    trigger = "rebuild",
    description = "Rebuild dependency that is currently set.",
    onStart = function()
        Log(0,"Rebuild : Starting process")
        if _OPTIONS["dependency"] == "all" then
            CleanAllDependencies()
            GenerateAllDependencies()
            BuildAllDependencies()
        else
            CleanDependency(FindDependencyByName{_OPTIONS["dependency"], true})
            GenerateDependency(FindDependencyByName{_OPTIONS["dependency"], true})
            BuildDependency(FindDependencyByName{_OPTIONS["dependency"], true})
        end
    end,

    onEnd = function()
        Log(0,"Rebuild : Finished")
    end,
}



return m
