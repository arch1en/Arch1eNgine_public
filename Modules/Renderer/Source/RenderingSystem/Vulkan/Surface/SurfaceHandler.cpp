#include "SurfaceHandler.h"
#include "SurfaceUtilities.h"

#ifdef PLATFORM_WIN32
#include "Implementations/Surface_SDL.h"
#endif

void SurfaceHandler::CreateSurface(const SurfaceHandlerCreationData* Data)
{
	Surface_Base* NewSurface = nullptr;

#ifdef PLATFORM_WIN32
	NewSurface = new Surface_SDL(Data);
#endif

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
