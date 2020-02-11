#ifndef ENGINEEDITOR_H
#define ENGINEEDITOR_H

#include "Mathematics.inl"

#include "imgui.h"
#include "imgui_internal.h"

class EngineEditor
{
public:

	void Initiate();
	void InitiateInput();
	void Render(Vector2<> DisplaySize);

private:

	ImGuiContext* Context;
};

#endif