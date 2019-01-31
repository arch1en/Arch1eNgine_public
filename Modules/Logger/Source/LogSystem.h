#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <vector>
#include <memory>

#include "LoggerImplementations/Logger_Impl.h"
#include "LoggerImplementations/Logger_General.h"
#include "LoggerImplementations/Logger_Vulkan.h"
#include "DebugTimer.h"

class LogSystem
{
public:

	LogSystem();

	static const LogSystem* const GetInstance();

	DebugTimer& GetTimer();

	Logger_General* const GetGeneralLogger() const;
	Logger_Vulkan* const GetVulkanLogger() const;

private:

	DebugTimer Timer;
	std::vector<std::unique_ptr<Logger_Impl>> Loggers;

};

#define LogV(LogType, LogDomain, VerbosityLevel, Message, ...) LogSystem::GetInstance()->GetGeneralLogger()->ExecuteLog(LogType, LogDomain, VerbosityLevel, Message, __VA_ARGS__);
#define Log(LogType, Message, ...) LogV(LogType, "Temp", 0, Message, __VA_ARGS__)
#define LogVk(LogType, VerbosityLevel, Message, ...) LogV(LogType, LOGDOMAIN_RENDERER_VULKAN, VerbosityLevel, Message, __VA_ARGS__);

#endif