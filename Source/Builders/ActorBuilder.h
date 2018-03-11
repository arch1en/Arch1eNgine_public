#pragma once

#include "stdafx.h"

#include "Actors\AActor.h"

class ActorBuilder
{
public:

	std::shared_ptr<AActor> NewActor(std::string aActorType);

};