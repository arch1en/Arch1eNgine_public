////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Most basic entity, allows one to see the world and receives input through component.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "stdafx.h"
#include "ACamera.h"

ACamera::ACamera()
{
	mCameraComponent = std::make_shared<CameraComponent>(this);
	if (mCameraComponent != nullptr)
	{
		BaseComponent* Component = static_cast<BaseComponent*>(mCameraComponent.get());
		mComponents.push_back(Component);
	}
	mInputComponent = std::make_shared<InputComponent>();
	if (mInputComponent != nullptr)
	{
		BaseComponent* Component = static_cast<BaseComponent*>(mInputComponent.get());
		mComponents.push_back(Component);
	}
	mMovementComponent = std::make_shared<MovementComponent>();
	if (mMovementComponent != nullptr)
	{
		BaseComponent* Component = static_cast<BaseComponent*>(mMovementComponent.get());
		mComponents.push_back(Component);
	}

	mInputComponent->BindContinuous("Input.Scene", this, &ACamera::InputListener);
	mInputComponent->BindMouseMotion("Input.Scene", this, &ACamera::MouseMotionListener);
	mInputComponent->SetGrabInput(false);
	mInputComponent->SetMouseSensitivity(0.1f);

	mMovementComponent->SetSpeed(0.1f);
	SetWorldPosition(glm::vec3(3.f, 0.f, 5.f));
}

ACamera::~ACamera()
{
	mCameraComponent.reset();
	mInputComponent.reset();
	mMovementComponent.reset();
}

void ACamera::InputListener(const KeyData& aKeyData)
{
	if (aKeyData.Command.compare("MoveForward") == 0)
	{
		mMovementComponent->SetDirection(GetForwardVector());
	}
	else if (aKeyData.Command.compare("MoveBackwards") == 0)
	{
		mMovementComponent->SetDirection(GetForwardVector() * -1.f);
	}
	else if (aKeyData.Command.compare("StrafeLeft") == 0)
	{
		mMovementComponent->SetDirection(GetRightVector() * -1.f);
	}
	else if (aKeyData.Command.compare("StrafeRight") == 0)
	{
		mMovementComponent->SetDirection(GetRightVector());
	}

	SetWorldPosition(GetWorldPosition() + mMovementComponent->GetCalculatedMovement());
}

void ACamera::MouseMotionListener(const MouseData& aMouseData)
{
	const glm::vec2& LastMousePosition = mInputComponent->GetLastMousePosition();
	
	auto OffsetX = GLfloat(aMouseData.DirectionX);
	auto OffsetY = GLfloat(-aMouseData.DirectionY);

	OffsetX *= mInputComponent->GetMouseSensitivity();
	OffsetY *= mInputComponent->GetMouseSensitivity();

	glm::vec3 NewRotation = GetWorldRotation();

	NewRotation.y += OffsetX;
	NewRotation.x += OffsetY;

	if (NewRotation.x > 89.0f) NewRotation.x = 89.0f;
	if (NewRotation.x < -89.0f) NewRotation.x = -89.0f;

	// [Rotation] ToDo : Rotation should also determine a facing direction !
	SetWorldRotation(NewRotation);
}

void ACamera::Update(float aDeltaTime)
{
	AActor::Update(aDeltaTime);


}

const std::shared_ptr<CameraComponent> ACamera::GetCameraComponent() const
{
	return mCameraComponent;
}