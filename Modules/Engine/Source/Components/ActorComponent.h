////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Actor Component
//  @author     : Artur Ostrowski
//  @usage      : Component allowed and designated only for actors.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#include "BaseComponent.h"

#include <glm/gtc/quaternion.hpp>
class AActor;

class ActorComponent : public BaseComponent
{
public:
	ActorComponent(AActor* aOwner )
		: BaseComponent()
		, mWorldPosition{ glm::vec3(0.0f, 0.0f, 0.0f) }
		, mWorldRotation{ glm::vec3(0.0f, 0.0f, 0.0f) }
		, mRelativePosition{ glm::vec3(0.0f, 0.0f, 0.0f) }
		, mRelativeRotation{ glm::vec3(0.0f, 0.0f, 0.0f) }
		, mUpVector{ glm::vec3(0.0f, 1.0f, 0.0f)}
		, mForwardVector{ glm::vec3(0.0f, 0.0f, -1.0f) }
		, mOwner{ aOwner }
	{}

	void SetWorldPosition(const glm::vec3 aPosition);
	void SetWorldRotation(const glm::vec3 aRotation);
	void SetFacingDirection(const glm::vec3 aDirection);

	void SetRelativePosition(const glm::vec3 aPosition);
	void SetRelativeRotation(const glm::vec3 aRotation);

	const glm::vec3 GetWorldPosition() const;
	const glm::vec3 GetWorldRotation() const;
	const glm::vec3 GetRelativePosition() const;
	const glm::vec3 GetRelativeRotation() const;

	const glm::vec3 GetUpVector() const;
	const glm::vec3 GetForwardVector() const;
	const glm::vec3 GetRightVector() const;
	
	AActor* GetOwner() const;

protected:
	glm::vec3 mWorldPosition;
	glm::vec3 mWorldRotation;
	glm::vec3 mRelativePosition;
	glm::vec3 mRelativeRotation;

	glm::vec3 mUpVector;
	glm::vec3 mForwardVector;

	virtual void Update(float aDeltaTime) override {}

private:
	AActor* mOwner;
};