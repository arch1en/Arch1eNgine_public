#include "SurfaceHandler.h"
#include "SurfaceUtilities.h"

#if defined(ARCHITECTURE_X86) || defined(ARCHITECTURE_X86_64) 
#include "Implementations/Surface_SDL.h"
#endif

#include "LogSystem.h"

void SurfaceHandler::CreateSurface(const SurfaceHandlerCreationData* Data)
{
	Surface_Base* NewSurface = nullptr;

#if defined(ARCHITECTURE_X86) || defined(ARCHITECTURE_X86_64) 
	NewSurface = new Surface_SDL(Data);
#endif

	Assert(NewSurface != nullptr, "Surface creation failed.");

	mSurfaces.push_back(std::unique_ptr<Surface_Base>(NewSurface));
}

void SurfaceHandler::DestroySurface(Surface_Base* Surface)
{
	Assert(Surface != nullptr, "Surface must be valid.");

	Surface->Destroy(*mInstanceHandleRef.get());
}

const Surface_Base* SurfaceHandler::GetMainSurface() const
{
	return mSurfaces[0].get();
}
