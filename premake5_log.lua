
LogVerbosity = tonumber(_OPTIONS["logverbosity"]) or 0

CurrentAction = ""

function Log(Verbosity, Message)
	local VerbosityIndentation = ""
	for i=1, Verbosity do
		VerbosityIndentation = VerbosityIndentation .. "    "
	end
    if Verbosity <= LogVerbosity then
        print(VerbosityIndentation .. Message)
    end
end

function LogAct(Verbosity, DependencyName, Message)
	Log(Verbosity, CurrentAction.. " ["..DependencyName.."] : "..Message)
end
