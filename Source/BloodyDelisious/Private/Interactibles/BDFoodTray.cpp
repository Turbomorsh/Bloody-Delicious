// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDFoodTray.h"

#include "Interactibles/BDPlate.h"

ABDFoodTray::ABDFoodTray()
{
    DrinkSocket = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    DrinkSocket->SetupAttachment(GetRootComponent());

    PlateSocket = CreateDefaultSubobject<USceneComponent>("SceneComponent1");
    PlateSocket->SetupAttachment(GetRootComponent());
}

void ABDFoodTray::Interact(TObjectPtr<UObject> Object)
{
    if (Cast<USceneComponent>(Object)) Super::Interact(Object);
    else if (Cast<ABDPickable>(Object))
    {
        PlaceItem(Object);
    }
}

FOrderStruct ABDFoodTray::GetTray()
{
    return TrayFood;
}

void ABDFoodTray::PlaceItem(TObjectPtr<UObject> Object)
{
    if (TObjectPtr<ABDPlate> CastedPlate = Cast<ABDPlate>(Object))
        if (CastedPlate->GetBurger().Num() != 0 && TrayFood.Burger.Num() == 0)
        {
            CastedPlate->Grab(PlateSocket);
            TrayFood.Burger = CastedPlate->GetBurger();
        }

    if (TObjectPtr<ABDCup> CastedCup = Cast<ABDCup>(Object))
        if (TrayFood.Drink == EDrinkType::Empty && CastedCup->Type != EDrinkType::Empty)
        {
            CastedCup->Grab(DrinkSocket);
            TrayFood.Drink = CastedCup->Type;
        }
}