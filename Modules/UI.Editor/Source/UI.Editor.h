#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include <SDL_video.h>
#include <Renderer_Base.h>
#include <AllocatorGPU.h>

namespace UI
{
	class Editor
	{
	public:

		void AttachContextToWindow(SDL_Window* Window);

	private:

		Renderer::Base mRenderer;
		AllocatorGPU mAllocatorGPU;
	}
}



#endif