
LogVerbosity = tonumber(_OPTIONS["logverbosity"]) or 0

CurrentAction = ""

function Log(Verbosity, Message)
    if Verbosity <= LogVerbosity then
        local IndentationLevel = ""
        for i=0,Verbosity,1 do
            IndentationLevel = IndentationLevel .. "    "
        end
        print("[Log]["..Verbosity.."]: " ..IndentationLevel..Message)
    end
end

function LogAct(Verbosity, DependencyName, Message)
	Log(Verbosity, CurrentAction.. " ["..DependencyName.."] : "..Message)
end
