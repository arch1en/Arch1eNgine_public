#pragma once

#include "Actors\AActor.h"

class FactoryActor
{
public:

	std::shared_ptr<AActor> NewActor(std::string aActorType);

};