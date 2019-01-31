
#include "LogSystem.h"

LogSystem::LogSystem()
{
	Loggers.resize(LogImplementation::TOTAL);
	Loggers[LogImplementation::General] = std::make_unique<Logger_General>();
	Loggers[LogImplementation::Vulkan] = std::make_unique<Logger_Vulkan>();
}

const LogSystem* const LogSystem::GetInstance()
{
	static LogSystem* Instance;
	if (!Instance)
	{
		Instance = new LogSystem;
	}
	return Instance;
}

Logger_General* const LogSystem::GetGeneralLogger() const
{
	if (Loggers[LogImplementation::General])
	{
		return static_cast<Logger_General*>(Loggers[LogImplementation::General].get());
	}

	return nullptr;
}

Logger_Vulkan* const LogSystem::GetVulkanLogger() const
{
	if (Loggers[LogImplementation::Vulkan])
	{
		return static_cast<Logger_Vulkan*>(Loggers[LogImplementation::Vulkan].get());
	}

	return nullptr;
}

DebugTimer& LogSystem::GetTimer()
{
	return Timer;
}
