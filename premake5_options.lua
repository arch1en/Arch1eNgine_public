newoption {
  trigger = "dependency",
  description = "Choose which dependency project should be taken into action (clean, build or rebuild).",
  default = "all",
  allowed = {
    { "all", "Everything will be taken into action." },
  }
}

-- [TODO] Options generated below are not showing on --help list. Fixit.
for i, v in ipairs(DependencyNames) do
  premake.option.list["dependency"].allowed[v:lower()] = "Only " ..v.. " library will be taken into action."
end
