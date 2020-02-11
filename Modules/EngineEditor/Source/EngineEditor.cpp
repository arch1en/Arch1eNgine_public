#include "EngineEditor.h"


void EngineEditor::Initiate()
{
	Context = ImGui::CreateContext();
	ImGui::SetCurrentContext(Context);
	ImGui::StyleColorsDark();
}

void EngineEditor::InitiateInput()
{
	Context->IO = ImGui::GetIO();
}

void EngineEditor::Render(Vector2<> DisplaySize)
{
	Context->IO.DisplaySize = { DisplaySize.x, DisplaySize.y };

	ImGui::NewFrame();

	ImGui::Render();
}