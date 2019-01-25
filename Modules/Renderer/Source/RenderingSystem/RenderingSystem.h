#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include "RenderingSystemUtilities.h"
#include "RenderingSystem/Instance/RenderingInstance_Impl.h"

#include <memory>

class RenderingSystem
{
public:
	void CreateInstance(RenderingInstanceProperties Properties);
	I::RenderingInstance_Impl* GetRenderingInstance() const;

private:

	std::unique_ptr<I::RenderingInstance_Impl> mRenderingInstance;
};

#endif