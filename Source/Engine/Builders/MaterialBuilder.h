#pragma once

#include <stdafx.h>

#include "Engine/Rendering/Material.h"

class MaterialBuilder
{
public:

	std::shared_ptr<Material> NewMaterial();

	std::shared_ptr<Material> NewDefaultMaterial();

};