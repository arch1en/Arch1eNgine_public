#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include "RenderingSystemUtilities.h"
#include "RenderingSystem/Context/RenderingContext_Impl.h"

#include <memory>

class RenderingSystem
{
public:
	void CreateContext(RenderingContextProperties Properties);
	I::RenderingContext_Impl* GetRenderingContext() const;

private:

	std::unique_ptr<I::RenderingContext_Impl> mRenderingContext;
};

#endif