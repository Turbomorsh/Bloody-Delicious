// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDBurgerPart.h"
#include "BDCup.h"
#include "Interactibles/BDPickable.h"
#include "BDFoodTray.generated.h"

/**
 *
 */

USTRUCT()
struct FOrderStruct
{
    GENERATED_BODY()

    EDrinkType Drink;

    TArray<EFoodType> Burger;

    friend bool operator==(const FOrderStruct a, const FOrderStruct b);
};

inline bool operator==(const FOrderStruct a, const FOrderStruct b)
{
    if (a.Drink == b.Drink && a.Burger == b.Burger) return true;
    else
        return false;
}

UCLASS()
class BLOODYDELISIOUS_API ABDFoodTray : public ABDPickable
{
    GENERATED_BODY()

public:
    ABDFoodTray();

    virtual void Interact(TObjectPtr<UObject> Object) override;

    FOrderStruct GetTray();

protected:
    void PlaceItem(TObjectPtr<UObject> Object);

    UPROPERTY()
    FOrderStruct TrayFood;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sockets)
    TObjectPtr<USceneComponent> PlateSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sockets)
    TObjectPtr<USceneComponent> DrinkSocket;
};
