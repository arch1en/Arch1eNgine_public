premake.modules.lua = {}

local m = premake.modules.lua
local p = premake

newaction {
  trigger = "build",
  description = "Build dependency that is currently set.",
  onStart = function()
    print("Build : Starting process")
  end,

  onWorkspace = function(wks)
    printf("Build : Workspace '%s' processing", wks.name)
  end,

  onProject = function(prj)
    printf("Build : Project '%s' processing", prj.name)
  end,

  execute = function()
    print("Build : Executing...")
  end,

  onEnd = function()
    print("Build : Ending process")
  end

}

newaction {
  trigger = "clean",
  description = "Clean dependency that is currently set.",
  onStart = function()
    print("Clean : Starting process")
  end,

  execute = function()
    print("Clean : Executing...")
  end,

  execute = function()
    print("Clean : Executing...")
  end,
}

newaction {
  trigger = "rebuild",
  description = "Rebuild dependency that is currently set.",
  onStart = function()
    print("Rebuild : Starting process")
  end,

  execute = function()
    print("Rebuild : Executing...")
  end,

  execute = function()
    print("Rebuild : Executing...")
  end,
}

return m
