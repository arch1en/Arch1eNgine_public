local DependencyAllowed = {}

DependencyAllowed [1] = { "all", "All libraries will be taken into action."}

for i, v in ipairs(Dependencies) do
  DependencyAllowed[i+1] = { v["Name"]:lower(), "Only " ..v["Name"].. " library will be taken into action." }
end

newoption {
  trigger = "dependency",
  description = "Choose which dependency project should be taken into action (clean, build or rebuild).",
  default = "all",
  allowed = DependencyAllowed
}
