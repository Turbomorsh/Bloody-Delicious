// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDBoxGiver.h"

#include "Player/BDPlayerCharacter.h"
#include "Interactibles/BDBox.h"
#include "Interactibles/BDFoodPartDataAsset.h"

// Sets default values
ABDBoxGiver::ABDBoxGiver()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(MeshComponent);
}
void ABDBoxGiver::Interact(TObjectPtr<UObject> Object)
{
    if (const TObjectPtr<USceneComponent> CastedScene = Cast<USceneComponent>(Object))
    {
        if (const TObjectPtr<ABDPlayerCharacter> CastedPlayer = Cast<ABDPlayerCharacter>(CastedScene->GetOwner())) GiveBox(CastedPlayer);
    }
}

// Called when the game starts or when spawned
void ABDBoxGiver::BeginPlay()
{
    Super::BeginPlay();
}

void ABDBoxGiver::GiveBox(TObjectPtr<ABDPlayerCharacter> Player)
{
    if (BoxClass)
    {
        const FTransform SpawnTransform = FTransform(FRotator(0), GetActorLocation());
        const FActorSpawnParameters SpawnParams;
        const TObjectPtr<ABDBox> NewBox = GetWorld()->SpawnActor<ABDBox>(BoxClass, SpawnTransform, SpawnParams);
        NewBox->PartType = FoodDataAsset->Type;
        Player->GrabItem(NewBox);
    }
}

// Called every frame
void ABDBoxGiver::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
