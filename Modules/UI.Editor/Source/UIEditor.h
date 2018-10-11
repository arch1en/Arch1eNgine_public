#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include "SDL2.h"

class UIEditor final
{
public:
	explicit UIEditor(Renderer* aRenderer);
	~UIEditor();

	void Initiate();

	void AttachContextToWindow(SDL_Window* SDL);

	void InvalidateDeviceObjects(mRenderer);

	std::shared_ptr<Renderer> mRenderer;
};

#endif