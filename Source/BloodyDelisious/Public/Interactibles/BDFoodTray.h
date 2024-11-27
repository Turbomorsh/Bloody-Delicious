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

USTRUCT(BlueprintType)
struct FOrderStruct
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, BlueprintType, EditAnywhere)
    TEnumAsByte<EDrinkType> Drink = EDrinkType::Empty;

    UPROPERTY(BlueprintReadWrite, BlueprintType, EditAnywhere)
    TArray<TEnumAsByte<EFoodType>> Burger;

    UPROPERTY(BlueprintReadWrite, BlueprintType, EditAnywhere, meta = (ClampMin = "0", ClampMax = "20"))
    int32 HorrorScore = 0;

    UPROPERTY(BlueprintReadWrite, BlueprintType, EditAnywhere, meta = (ClampMin = "-20", ClampMax = "0"))
    int32 AntiHorrorScore = 0;

    UPROPERTY(BlueprintReadWrite, BlueprintType, EditAnywhere, meta = (ClampMin = "0", ClampMax = "20"))
    int32 FineScore = 2;

    FOrderStruct()
    {
        Drink = EDrinkType::Empty;

        const TArray<TEnumAsByte<EFoodType>> NewBurger;
        Burger = NewBurger;
    }

    FOrderStruct(const TEnumAsByte<EDrinkType> InDrink, const TArray<TEnumAsByte<EFoodType>> InBurger)
    {
        Drink = InDrink;

        Burger = InBurger;
    }

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
