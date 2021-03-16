#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class USphereComponent;
class USkeletalMeshComponent;

UCLASS()
class COLOSSEUMPROJECT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	// TODO: now it's done only for weapon, then make separate class for ammo, health, armor
	UPROPERTY(VisibleAnywhere, Category = "Item")
	USkeletalMeshComponent* ItemMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	USphereComponent* PickupSphere;

	float CustomSphereRadius;
	

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);	
	

};
