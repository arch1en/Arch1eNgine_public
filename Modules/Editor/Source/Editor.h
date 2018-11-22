#ifndef EDITOR_EDITOR_H
#define EDITOR_EDITOR_H

class UI::Editor;
class Renderer::Base;

namespace Editor
{
	class Editor
	{


	public:

		Editor() = default;
		void Initiate();
		bool InitiateRenderer();

		UI::Editor* EditorUI;
		Renderer::Base* Renderer;

	private:

		SDL_Window* Window;
	};
}


#endif