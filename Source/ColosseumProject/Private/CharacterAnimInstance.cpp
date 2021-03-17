// Colosseum Project.
#include "CharacterAnimInstance.h"
#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	GetAnimOwner();

	MovementTransition = 10.f;
}

void UCharacterAnimInstance::UpdateAnimationProperties()
{
	if (!GetAnimOwner()) return;

	bIsInCrouch = Pawn->GetMovementComponent()->IsCrouching();
	bIsAiming = Cast<ACharacterBase>(Pawn)->IsAiming();

	UpdateMovement();

	if (bIsAiming)
		UpdateAiming();	
}

bool UCharacterAnimInstance::GetAnimOwner()
{	
	Pawn = TryGetPawnOwner();
	return Pawn;
}

void UCharacterAnimInstance::UpdateMovement()
{
	FVector Speed = Pawn->GetVelocity();
	FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
	MovementSpeed = LateralSpeed.Size();

	FRotator CurrentRotation = Pawn->GetActorRotation();
	MovementDirection = CalculateDirection(Speed, CurrentRotation);
}

void UCharacterAnimInstance::UpdateAiming()
{
	FRotator ControlRotation = Pawn->GetControlRotation();
	FRotator PawnRotation = Pawn->GetActorRotation();
	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, PawnRotation);
	FRotator AimRotation(0.f, AimPitch, AimYaw);

	FRotator InterpolatedRotation = FMath::RInterpTo(AimRotation, DeltaRotator, GetWorld()->GetTimeSeconds(), 15.f);

	float DummyRoll = 0.f;
	float TempYaw = 0.f;
	float TempPitch = 0.f;

	UKismetMathLibrary::BreakRotator(InterpolatedRotation, DummyRoll, TempPitch, TempYaw);
	
	AimPitch = FMath::ClampAngle(TempPitch, MinAimingAngle, MaxAimingAngle);
	AimYaw = FMath::ClampAngle(TempYaw, MinAimingAngle, MaxAimingAngle);
}