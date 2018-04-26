premake.modules.lua = {}

local m = premake.modules.lua
local p = premake

newaction {
  trigger = "generate",
  description = "Generate dependency that is currently set.",
  onStart = function()
    if _OPTIONS["dependency"] == "assimp" then
      generate_assimp()
    elseif _OPTIONS["dependency"] == "devil" then
      generate_devil()
    elseif _OPTIONS["dependency"] == "freetype" then
      generate_freetype()
    elseif _OPTIONS["dependency"] == "glew" then
      generate_glew()
    elseif _OPTIONS["dependency"] == "sdl2" then
      generate_sdl2()
    elseif _OPTIONS["dependency"] == "all" then
      generate_all()
    end
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
    if _OPTIONS["dependency"] == "assimp" then
      build_assimp()
    elseif _OPTIONS["dependency"] == "devil" then
      build_devil()
    elseif _OPTIONS["dependency"] == "freetype" then
      build_freetype()
    elseif _OPTIONS["dependency"] == "glew" then
      build_glew()
    elseif _OPTIONS["dependency"] == "sdl2" then
      build_sdl2()
    elseif _OPTIONS["dependency"] == "all" then
      build_all()
    end
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
    if _OPTIONS["dependency"] == "assimp" then
      clean_assimp()
    elseif _OPTIONS["dependency"] == "devil" then
      clean_devil()
    elseif _OPTIONS["dependency"] == "freetype" then
      clean_freetype()
    elseif _OPTIONS["dependency"] == "glew" then
      clean_glew()
    elseif _OPTIONS["dependency"] == "sdl2" then
      clean_sdl2()
    elseif _OPTIONS["dependency"] == "all" then
      clean_all()
    end
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

  end,

  --execute = function()
  --  print("Rebuild : Executing...")
  --end,

  onEnd = function()
    print("Rebuild : Finished")
  end,
}

return m
