// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDTray.h"

#include "Interactibles/BDBurgerPart.h"
#include "Interactibles/BDBox.h"
#include "Interactibles/BDFoodPartDataAsset.h"
#include "Kismet/GameplayStatics.h"
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

void ABDTray::Scream()
{
    TransformToAlterFood();
}

void ABDTray::DisableScream() {}

void ABDTray::BeginPlay()
{
    Super::BeginPlay();
}

void ABDTray::TransformToAlterFood()
{
    TArray<AActor*> AllFood;
    UGameplayStatics::GetAllActorsOfClass(this, FoodClass, AllFood);

    for (AActor* ThisFood : AllFood)
    {
        if (TObjectPtr<ABDBurgerPart> CastedFood = Cast<ABDBurgerPart>(ThisFood))
        {
            if (FoodDataAsset)
                if (CastedFood->PartType == FoodDataAsset->Type)
                {
                    CastedFood->Scream();
                }
        }
    }
}
void ABDTray::TransformToNormalFood()
{
    // TArray<AActor*> AllFood;
    // UGameplayStatics::GetAllActorsOfClass(this, FoodClass, AllFood);

    //    for (AActor* ThisFood : AllFood)
    //  {
    //    TObjectPtr<ABDBurgerPart> CastedFood = Cast<ABDBurgerPart>(ThisFood);
    //
    //      if (CastedFood->PartType == FoodDataAsset->AlterTransform->Type)
    //    {
    //      CastedFood->;
    //}
    //}
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

void ABDTray::ChangeTrayType(TObjectPtr<UBDFoodPartDataAsset> NewType)
{
    FoodDataAsset = NewType;
}