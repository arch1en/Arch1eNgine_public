#pragma once

#include <stdafx.h>

#include <map>

class Material;

class MaterialObjectPool
{
public:

	const Material*	GetMaterialByName(std::string aName) const;
	void AddUniqueMaterial(std::string aName, Material aMaterial);

private:

	std::map<std::string, std::unique_ptr<Material>> mMaterials;

};