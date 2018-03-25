#include "MaterialObjectPool.h"

#include "Engine/Rendering/Material.h"

const Material*	MaterialObjectPool::GetMaterialByName(std::string aName) const
{
	return mMaterials.at(aName).get();
}

void MaterialObjectPool::AddUniqueMaterial(std::string aName, Material aMaterial)
{
	mMaterials.insert(std::make_pair(aName, std::make_unique<Material>(aMaterial)));
}