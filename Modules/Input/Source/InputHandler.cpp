////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Component that allows :
//					- Registering to input layers.
//					- Sends input with help of InputLayers and InputModule to interested delegates.
//  @version    : 1.0.0
//
////////////////////////////////////////
//#include "stdafx.h"
#include "InputHandler.h"
#include "Assertions.inl"

InputHandler::InputHandler(Input* aInputModule)
	: mInput{ aInputModule }
{

}

InputHandler::~InputHandler()
{
	for (const std::string& Layer : mLayersBoundTo)
	{
		Assert(mInput,"Input is null");

		mInput->UnregisterFromLayer(this, Layer);
	}
}

void InputHandler::RegisterToLayer(const std::string& aLayerName)
{
	Assert(mInput, "Input is null");

	bool RegisterResult = mInput->RegisterToLayer(this, aLayerName);

	if (RegisterResult == true)
	{
		auto& Result = std::find(mLayersBoundTo.begin(), mLayersBoundTo.end(), aLayerName);

		if (Result == mLayersBoundTo.end())
		{
			mLayersBoundTo.push_back(aLayerName);
		}
	}
}

void InputHandler::SetGrabInput(bool aState)
{
	SDL_SetRelativeMouseMode(aState ? SDL_TRUE : SDL_FALSE);
}