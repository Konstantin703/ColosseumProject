#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CharacterBase.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(GetRootComponent());

	CustomSphereRadius = 100;
	PickupSphere->SetSphereRadius(CustomSphereRadius);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	// TODO: toggle collision when picking up/drop weapon
	PickupSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnOverlapEnd);
	
}

void AItemBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
)
{
	if (OtherActor)
	{
		ACharacterBase* OverlappedCharacter = Cast<ACharacterBase>(OtherActor);
		if (OverlappedCharacter)
		{
			// don't remove this log for debugging of overlapped NPC when they appear
			UE_LOG(LogTemp, Warning, TEXT("Overlapped with %s of class %s"), *OverlappedCharacter->GetName(), *OverlappedCharacter->GetClass()->GetName());
			OverlappedCharacter->AddToLootArray(this);
		}		
	}	
}

void AItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACharacterBase* OverlappedCharacter = Cast<ACharacterBase>(OtherActor);
		if (OverlappedCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("End Overlap with %s of class %s"), *OverlappedCharacter->GetName(), *OverlappedCharacter->GetClass()->GetName());
			OverlappedCharacter->RemoveFromLootArray(this);
		}
	}
}

