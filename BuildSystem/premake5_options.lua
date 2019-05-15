-- Dependency Option

local DependencyAllowed = {}

DependencyAllowed [1] = { "all", "All libraries will be taken into action."}

for i, v in ipairs(GetListOfDependencyNames()) do
	DependencyAllowed[i+1] = { v:lower(), "Only " ..v.. " library will be taken into action." }
end

newoption {
  trigger = "dependency",
  description = "Choose which dependency project should be taken into action (clean, build, rebuild or organize).",
  default = "all",
  allowed = DependencyAllowed
}

local ConfigurationsAllowed = {}

for i,v in ipairs(Configurations) do
    ConfigurationsAllowed[i] = { v.Name, "Build dependency in " ..v.Name.. " configuration." }
end

newoption {
    trigger = "configuration",
    description = "Choose which configuration of selected dependency should be taken into action.",
    default = "Debug",
    allowed = ConfigurationsAllowed 
}

local PlatformsAllowed = {}

for i,v in ipairs(Platforms) do
    PlatformsAllowed[i] = { v.Name, "Build dependency on " ..v.Name.. " platform." }
end

newoption {
    trigger = "platform",
    description = "Choose which platform of selected configuration and dependency should be takein into action.",
    default = "Win64",
    allowed = PlatformsAllowed
}

newoption {
    trigger = "logverbosity",
    description = "How verbose should logging system be.",
    default = "0",
    allowed = { 
        {"0", "Least Verbose" },
        {"1", "Verbose"},
        {"2", "Very Verbose"}
   }
}
