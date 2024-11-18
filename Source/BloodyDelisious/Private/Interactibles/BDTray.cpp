// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDTray.h"

#include "Interactibles/BDBurgerPart.h"
#include "Interactibles/BDBox.h"
#include "Interactibles/BDFoodPartDataAsset.h"
#include "Player/BDPlayerCharacter.h"

ABDTray::ABDTray()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(MeshComponent);
}

void ABDTray::Interact(TObjectPtr<UObject> Object)
{
    if (UsageLeft == 0)
    {
        if (TObjectPtr<ABDBox> CastedBox = Cast<ABDBox>(Object))
        {
            if (CastedBox->PartType == FoodDataAsset->Type)
            {
                UsageLeft = MaxUsage;
                CastedBox->Destroy();
            }
        }
    }
    else if (TObjectPtr<USceneComponent> CastedScene = Cast<USceneComponent>(Object))
    {
        if (TObjectPtr<ABDPlayerCharacter> CastedPlayer = Cast<ABDPlayerCharacter>(CastedScene->GetOwner())) TakeItem(CastedPlayer);
    }
}

void ABDTray::BeginPlay()
{
    Super::BeginPlay();
}
void ABDTray::TakeItem(TObjectPtr<ABDPlayerCharacter> Player)
{
    if (FoodClass && FoodDataAsset)
    {
        const FTransform SpawnTransform = FTransform(FRotator(0), GetActorLocation());
        FActorSpawnParameters SpawnParams;
        TObjectPtr<ABDBurgerPart> NewFoodPart = GetWorld()->SpawnActor<ABDBurgerPart>(FoodClass, SpawnTransform, SpawnParams);
        NewFoodPart->ChangeType(FoodDataAsset);
        Player->GrabItem(NewFoodPart);

        UsageLeft--;
    }
}