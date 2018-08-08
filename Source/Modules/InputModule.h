////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Input Module
//  @author     : Artur Ostrowski
//  @usage      : Important part of the project. Input module is responsible for managing activity of input layers.
//					Intermittet - Input response that launches delegates only if a key is pressed. Input behavior is similar to text typing.
//					Continuous - Input response that is listened all the time, but launches delegates only if a key is pressed. Input behaviour is continous.
//  @version    : 1.0.1
//
//	TODO : Probably there will be need to sort active layers (eg. when 2 or more layers with the same key are active, which one should respond ?).
//
////////////////////////////////////////
#pragma once

#include "IO/InputLayer.h"

#include <unordered_map>

class InputComponent;

class InputModule
{
friend class ModuleHandler;

public:
	InputModule();
	InputModule(const InputModule&) = delete;
	InputModule operator= (const InputModule& RHS) = delete;
	~InputModule();

	static InputModule& GetInstance()
	{
		static InputModule Module;
		return Module;
	}

	bool	RegisterToLayer(InputComponent* aInputComponent, std::string aLayerCategory);
	void	UnregisterFromLayer(InputComponent* aInputComponent, std::string aLayerCategory);

private:

	std::shared_ptr<InputLayer>			EnsuredRegisterToLayer(InputComponent* aInputComponent, std::string aLayerCategory);

	std::unordered_map<std::string, std::shared_ptr<InputLayer>>	mActiveLayers;
	std::unordered_map<std::string, std::shared_ptr<InputLayer>>	mInputLayers;

	bool IsLayerCreated(const std::string& aLayerCategory);

	std::shared_ptr<InputLayer> CreateInputLayer(const std::string& aLayerCategory);
	void DestroyInputLayer(const std::string& aLayerCategory);

	void ActivateLayer();
	void DeactivateLayer();

	void InitializeLayers();
	/**
	*   Update intermittet delegates.
	*
	*   @param  aEvent    : Input event generated by SDL upon pressing button.
	*
	*/
	void Update(SDL_Event& aEvent);

	/**
	*   Update continuous delegates.
	*/
	void Update();

	//	void SetCurrentLayer(std::shared_ptr<InputLayer> InLayer, InputLayerType InLayerType);

	////////////////////////////////////////
	//	Mouse
	////////////////////////////////////////

	int mLastMouseX;
	int mLastMouseY;
};