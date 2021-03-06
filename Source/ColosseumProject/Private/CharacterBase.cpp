// Colosseum Project.
#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ItemBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(32.f, 90.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// for crouch
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	bAiming = false;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpAtRate);

	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ACharacterBase::ToggleCrouch);
	PlayerInputComponent->BindAction("Pickup", EInputEvent::IE_Pressed, this, &ACharacterBase::PickupWeapon);

	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACharacterBase::ToggleAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACharacterBase::ToggleAim);
}

void ACharacterBase::MoveForward(float InValue)
{
	if (Controller && (InValue != 0.f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, InValue);
	}
}

void ACharacterBase::MoveRight(float InValue)
{
	if (Controller && (InValue != 0.f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, InValue);
	}
}

void ACharacterBase::TurnAtRate(float InRate)
{
	AddControllerYawInput(InRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUpAtRate(float InRate)
{
	AddControllerPitchInput(InRate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::ToggleCrouch()
{
	GetCharacterMovement()->IsCrouching() ? UnCrouch(true) : Crouch(true);
}

void ACharacterBase::ToggleAim()
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming"));
	bAiming = !bAiming;

	// TODO: make smoother turn of chracter to camera
	if (bAiming)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}	
}

void ACharacterBase::AddToLootArray(AActor* ItemToAdd)
{
	if (!ItemToAdd || LootArray.Contains(ItemToAdd)) return;

	LootArray.Add(ItemToAdd);
}

void ACharacterBase::RemoveFromLootArray(AActor* ItemToRemove)
{
	if (!ItemToRemove || !LootArray.Contains(ItemToRemove)) return;

	LootArray.Remove(ItemToRemove);
}

//** TODO: make logic of finding the closest weapon on the floor ( or weapon on which camera is looking at) and picking it up */
void ACharacterBase::PickupWeapon()
{
	if (LootArray.Num() == 0) return;



	if (EquippedWeapon)
	{
		AItemBase* Temp = EquippedWeapon;
		Temp->Destroy();
		EquippedWeapon = nullptr;
	}

	AItemBase* Weapon = Cast<AItemBase>(LootArray[0]);
	if (Weapon)
	{		
		Weapon->GetItemMesh()->SetSimulatePhysics(false);
		SetEquippedWeapon(Weapon);
		// TODO: socket name into settings  
		const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName("WeaponSocket");
		if (WeaponSocket)
		{			
			WeaponSocket->AttachActor(LootArray[0], GetMesh());
			RemoveFromLootArray(Weapon);
		}
	}
}