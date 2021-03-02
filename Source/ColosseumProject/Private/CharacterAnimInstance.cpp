// Colosseum Project.
#include "CharacterAnimInstance.h"
#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	GetAnimOwner();

	MovementTransition = 10.f;
}

void UCharacterAnimInstance::UpdateAnimationProperties()
{
	if (!GetAnimOwner()) return;

	FVector Speed = Pawn->GetVelocity();
	FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
	MovementSpeed = LateralSpeed.Size();

	bIsInCrouch = Pawn->GetMovementComponent()->IsCrouching();	
}

bool UCharacterAnimInstance::GetAnimOwner()
{	
	Pawn = TryGetPawnOwner();
	return Pawn;
}