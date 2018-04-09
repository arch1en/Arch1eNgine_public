
newoption {
  trigger = "dependency",
  description = "Choose which dependency project should be taken into action (clean, build or rebuild).",
  default = "all"
  allowed = {
    { "all", "Everything will be taken into action." },
    { "assimp", "Only ASSIMP library will be taken into action." },
    { "devil", "Only DevIL library will be taken into action." },
    { "freetype", "Only FreeType library will be taken into action." },
    { "glew", "Only GLEW library will be taken into action." },
    { "sdl2", "Only SDL2 library will be taken into action." },
  }
}
