#include "stdafx.h"
#include "FactoryActor.h"

#include "Actors\PrimitiveActors\ATriangle.h"
#include "Actors\PrimitiveActors\ACube.h"

std::shared_ptr<AActor> FactoryActor::NewActor(std::string aActorType)
{
	std::shared_ptr<AActor> Actor = nullptr;

	if (aActorType.compare("triangle") == 0)
	{
		Actor = std::make_shared<ATriangle>();
		if (Actor == nullptr)
		{
			Log(DebugType::EDT_Error, "Error creating new actor.");
			return nullptr;
		}

		return Actor;
	}
	else if (aActorType.compare("cube") == 0)
	{
		Actor = std::make_shared<ACube>();
		if (Actor == nullptr)
		{
			Log(DebugType::EDT_Error, "Error creating new actor.");
			return nullptr;
		}

		return Actor;
	}

	return nullptr;
}