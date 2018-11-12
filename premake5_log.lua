
LogVerbosity = tonumber(_OPTIONS["logverbosity"]) or 0

function Log(Verbosity, Message)
    if Verbosity <= LogVerbosity then
        print(Message)
    end
end
