////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Helpers
//  @author     : Artur Ostrowski
//  @usage      : Miscellaneous helper functions.
//  @version    : 1.0.0
//
////////////////////////////////////////

#include "Helpers.h"

//#include "stdafx.h"
#include <string>
#include <sstream>

void Split(const char * InSentence, char InDelimiter, std::vector<std::string> & OutTokens)
{
	std::stringstream SStream;
	SStream.str(InSentence);

	std::string Item;
	while (std::getline(SStream, Item, InDelimiter))
	{
		OutTokens.push_back(Item);
	}
}

std::string GetFileExtension(const char* aFilePath)
{
	std::vector<std::string> OutTokens;
	Split(aFilePath, '.', OutTokens);
	if (OutTokens.size() == 2)
	{
		return OutTokens[1];
	}
	else
	{
		return "";
	}
}

void TranslateInputKeyToSDLKeycode(const std::string& aKey, char& aOutChar)
{
	if (aKey.compare("space") == 0) { aOutChar = char(SDLK_SPACE); return; }
	if (aKey.compare("lshift") == 0) { aOutChar = char(SDLK_LSHIFT); return; }
	if (aKey.compare("rshift") == 0) { aOutChar = char(SDLK_RSHIFT); return; }
	if (aKey.compare("lctrl") == 0) { aOutChar = char(SDLK_LCTRL); return; }
	if (aKey.compare("rctrl") == 0) { aOutChar = char(SDLK_RCTRL); return; }
	if (aKey.compare("lalt") == 0) { aOutChar = char(SDLK_LALT); return; }
	if (aKey.compare("ralt") == 0) { aOutChar = char(SDLK_RALT); return; }
	if (aKey.compare("enter") == 0 || aKey.compare("return") == 0) { aOutChar = char(SDLK_RETURN); return; }
	if (aKey.compare("esc") == 0 || aKey.compare("escape") == 0) { aOutChar = char(SDLK_ESCAPE); return; }
	if (aKey.compare("f1") == 0) { aOutChar = char(SDLK_F1); return; }
	if (aKey.compare("f2") == 0) { aOutChar = char(SDLK_F2); return; }
	if (aKey.compare("f3") == 0) { aOutChar = char(SDLK_F3); return; }
	if (aKey.compare("f4") == 0) { aOutChar = char(SDLK_F4); return; }
	if (aKey.compare("f5") == 0) { aOutChar = char(SDLK_F5); return; }
	if (aKey.compare("f6") == 0) { aOutChar = char(SDLK_F6); return; }
	if (aKey.compare("f7") == 0) { aOutChar = char(SDLK_F7); return; }
	if (aKey.compare("f8") == 0) { aOutChar = char(SDLK_F8); return; }
	if (aKey.compare("f9") == 0) { aOutChar = char(SDLK_F9); return; }
	if (aKey.compare("f10") == 0) { aOutChar = char(SDLK_F10); return; }
	if (aKey.compare("f11") == 0) { aOutChar = char(SDLK_F11); return; }
	if (aKey.compare("f12") == 0) { aOutChar = char(SDLK_F12); return; }
}