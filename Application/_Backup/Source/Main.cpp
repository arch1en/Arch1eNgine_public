#include "stdafx.h"

//#include "Engine/Engine.h"
#undef main
//Engine GEngine;
#include "Application.h"

int main(int argc, char** argv[])
{
//#if EDITOR_INCLUDED
	Application App;
	App.Initiate();
	App.CreateApplicationWindow();
	App.Loop();
//#endif

	return 0;
}