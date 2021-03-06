// Colosseum Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItemBase;

UCLASS()
class COLOSSEUMPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();	

	void AddToLootArray(AActor* ItemToAdd);
	void RemoveFromLootArray(AActor* ItemToRemove);

	FORCEINLINE void SetEquippedWeapon(AItemBase* WeaponToEquip) { EquippedWeapon = WeaponToEquip; }

	FORCEINLINE bool IsAiming() { return bAiming; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AItemBase* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Loot")
	TArray<AActor*> LootArray;

	bool bAiming;

	void MoveForward(float InValue);
	void MoveRight(float InValue);
	
	void TurnAtRate(float InRate);
	void LookUpAtRate(float InRate);

	void ToggleCrouch();

	void ToggleAim();
	//** Picking up means equiping too, if weapon equipped drop it and equip new */
	void PickupWeapon();

};
