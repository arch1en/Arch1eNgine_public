//#include "stdafx.h"
#include "ActorComponent.h"

AActor* ActorComponent::GetOwner() const
{
	return mOwner;
}

void ActorComponent::SetWorldPosition(const glm::vec3 aPosition)
{
	mWorldPosition = aPosition;
}

void ActorComponent::SetWorldRotation(const glm::vec3 aRotation)
{
	mWorldRotation = aRotation;
	glm::vec3 NewFrontVector;

	NewFrontVector.x = cos(glm::radians(aRotation.y)) * cos(glm::radians(aRotation.x));
	NewFrontVector.y = sin(glm::radians(aRotation.x));
	NewFrontVector.z = sin(glm::radians(aRotation.y)) * cos(glm::radians(aRotation.x));

	mForwardVector = glm::normalize(NewFrontVector);
}

void ActorComponent::SetFacingDirection(const glm::vec3 aDirection)
{
	mForwardVector = glm::normalize(aDirection);
}

void ActorComponent::SetRelativePosition(const glm::vec3 aPosition)
{
	mWorldPosition = mWorldPosition + aPosition;
	mRelativePosition = aPosition;
}

void ActorComponent::SetRelativeRotation(const glm::vec3 aRotation)
{
	mWorldRotation = mWorldRotation + aRotation;
	mRelativeRotation = aRotation;
}

const glm::vec3 ActorComponent::GetWorldPosition() const
{
	return mWorldPosition;
}
const glm::vec3 ActorComponent::GetWorldRotation() const
{
	return mWorldRotation;
}

const glm::vec3 ActorComponent::GetRelativePosition() const
{
	return mRelativePosition;
}
const glm::vec3 ActorComponent::GetRelativeRotation() const
{
	return mRelativeRotation;
}

const glm::vec3 ActorComponent::GetUpVector() const
{
	return mUpVector;
}

const glm::vec3 ActorComponent::GetForwardVector() const
{
	return mForwardVector;
}

const glm::vec3 ActorComponent::GetRightVector() const
{
	return glm::normalize(glm::cross(mUpVector, mForwardVector) * -1.f);
}

