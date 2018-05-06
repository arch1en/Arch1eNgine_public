premake.modules.lua = {}

local m = premake.modules.lua
local p = premake

newaction {
  trigger = "generate",
  description = "Generate dependency that is currently set.",
  onStart = function()
      GenerateDependency(_OPTIONS["dependency"] )
  end,

  onEnd = function()
    print("Generate : Finished")
  end
}

newaction {
  trigger = "build",
  description = "Build dependency that is currently set.",
  onStart = function()
    print("Build : Starting process")
    BuildDependency(_OPTIONS["dependency"])
  end,

  --onWorkspace = function(wks)
  --  printf("Build : Workspace '%s' processing", wks.name)
  --end,

  --onProject = function(prj)
  --  printf("Build : Project '%s' processing", prj.name)
  --end,

  --execute = function()
  --  print("Build : Executing...")
  --end,

  onEnd = function()
    print("Build : Finished")
  end

}

newaction {
  trigger = "clean",
  description = "Clean dependency that is currently set.",
  onStart = function()
    print("Clean : Starting process")
      CleanDependency(_OPTIONS["dependency"])
  end,

  --execute = function()
  --  print("Clean : Executing...")
  --end,

  onEnd = function()
    print("Clean : Finished")
  end,
}

newaction {
  trigger = "rebuild",
  description = "Rebuild dependency that is currently set.",
  onStart = function()
    print("Rebuild : Starting process")
    clean_dependency(_OPTIONS["dependency"])
    generate_dependency(_OPTIONS["dependency"])
    build_dependency(_OPTIONS["dependency"])
  end,

  --execute = function()
  --  print("Rebuild : Executing...")
  --end,

  onEnd = function()
    print("Rebuild : Finished")
  end,
}

return m
