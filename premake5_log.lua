
LogVerbosity = tonumber(_OPTIONS["logverbosity"]) or 0

CurrentAction = ""

function Log(Verbosity, Message)
    if Verbosity <= LogVerbosity then
        print(Message)
    end
end

function LogAct(Verbosity, DependencyName, Message)
	Log(Verbosity, CurrentAction.. " ["..DependencyName.."] : "..Message)
end
