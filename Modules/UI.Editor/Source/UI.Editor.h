
#ifndef UI_EDITOR_H
#define UI_EDITOR_H

namespace UI
{
	class Editor
	{
	public:

		void AttachContextToWindow(SDL_Window* Window);

	private:

		Renderer mRenderer;
		AllocatorGPU mAllocatorGPU;
	}
}



#endif