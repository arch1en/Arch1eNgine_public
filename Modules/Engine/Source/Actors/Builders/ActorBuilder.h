#pragma once

#include "Actors\AActor.h"

class ActorBuilder
{
public:

	template<typename T>
	std::shared_ptr<T> NewActor()
	{
		static_assert(std::is_base_of<AActor, T>::value, "Cannot create new actor that is not deriving from AActor.");

		std::shared_ptr<T> Actor = nullptr;

		Actor = std::make_shared<T>();
		if (Actor == nullptr)
		{
			Log(DebugType::EDT_Error, "Error creating new actor.");
			return nullptr;
		}

		return Actor;
	}

};