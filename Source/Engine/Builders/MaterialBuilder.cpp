#include "MaterialBuilder.h"

std::shared_ptr<Material> MaterialBuilder::NewMaterial()
{
	std::shared_ptr<Material> CreatedMaterial = std::make_shared<Material>();

	return CreatedMaterial;
}

std::shared_ptr<Material> MaterialBuilder::NewDefaultMaterial()
{
	std::shared_ptr<Material> CreatedMaterial = NewMaterial();

	CreatedMaterial->SetDiffuseColor(Color::Grey);
	CreatedMaterial->SetSpecularColor(Color::Grey);

	return CreatedMaterial;
}