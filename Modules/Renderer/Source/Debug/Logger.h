#ifndef RENDERER_DEBUG_LOGGER_H
#define RENDERER_DEBUG_LOGGER_H

#include "Debug/Logger.h"
#include "vulkan/vulkan.h"

#include "Debug/LoggerImpl/Logger_Vulkan.h"

Logger_Vulkan* const GetVulkanLogger() const;


#define LogVk(LogType, VerbosityLevel, Message, ...) LogV(LogType, LOGDOMAIN_RENDERER_VULKAN, VerbosityLevel, Message, __VA_ARGS__);


#endif
