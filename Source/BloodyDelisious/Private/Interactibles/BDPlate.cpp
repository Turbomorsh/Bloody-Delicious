// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDPlate.h"

#include "Interactibles/BDBurgerPart.h"

ABDPlate::ABDPlate()
{
    FoodSocket = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    FoodSocket->SetupAttachment(GetRootComponent());
}

void ABDPlate::Interact(TObjectPtr<UObject> Object)
{
    if (Cast<USceneComponent>(Object)) Super::Interact(Object);
    else if (TObjectPtr<ABDBurgerPart> CastedPart = Cast<ABDBurgerPart>(Object))
        PlaceItem(CastedPart);
}
TArray<TEnumAsByte<EFoodType>> ABDPlate::GetBurger()
{
    return TypesArray;
}

void ABDPlate::BeginPlay()
{
    Super::BeginPlay();
}

void ABDPlate::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABDPlate::PlaceItem(TObjectPtr<ABDBurgerPart> BurgerPart)
{
    if (BurgerPart)
    {
        // BurgerPart->Grab(LastPart ? FoodSocket : LastPart->FoodSocket);

        if (LastPart)
        {
            BurgerPart->Grab(LastPart->FoodSocket);
        }
        else
        {
            BurgerPart->Grab(FoodSocket);
        }

        TypesArray.Add(BurgerPart->PartType);
        BurgerPart->SetActorEnableCollision(false);
        LastPart = BurgerPart;
    }
}