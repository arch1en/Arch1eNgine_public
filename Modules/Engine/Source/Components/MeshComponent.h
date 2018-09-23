#pragma once

#include "ActorComponent.h"

class Mesh;

class MeshComponent : public ActorComponent
{
public:
	MeshComponent(AActor* aOwner)
		:ActorComponent{ aOwner }
	{}
	void SetMesh(std::shared_ptr<Mesh> aMesh);
	std::shared_ptr<Mesh> GetMesh() const;
	virtual void Update(float aDeltaTime) override;

private:

	std::shared_ptr<Mesh>	mMesh;

};