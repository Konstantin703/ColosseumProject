// Colosseum Project.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class COLOSSEUMPROJECT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementDirection;
	//** value to define when character starts/stops moving */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementTransition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	APawn* Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	ACharacterBase* Character;

private:
	bool GetAnimOwner();
	
};
