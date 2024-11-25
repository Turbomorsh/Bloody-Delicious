// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/BDPickable.h"
#include "BDBurgerPart.generated.h"

class UBDFoodPartDataAsset;
UENUM(BlueprintType)
enum EFoodType : uint8
{
    // clang-format off
    Cheese = 0              UMETA(DisplayName = "Cheese"),
    Salad                   UMETA(DisplayName = "Salad"),
    Tomato                  UMETA(DisplayName = "Tomato"),
    Cucumber                UMETA(DisplayName = "Cucumber"),
    Onion                   UMETA(DisplayName = "Onion"),
    TopBun                  UMETA(DisplayName = "Top Bun"),
    TopBurnedBun            UMETA(DisplayName = "Top Burned Bun"),
    TopCookedBun            UMETA(DisplayName = "Top Cooked Bun"),
    Meat                    UMETA(DisplayName = "Meat"),
    CookedMeat              UMETA(DisplayName = "Cooked Meat"),
    BurnedMeat              UMETA(DisplayName = "Burned Meat"),
    SauceYellow             UMETA(DisplayName = "Sauce Yellow"),
    SauceRed                UMETA(DisplayName = "Sauce Red"),
    SauceHot                UMETA(DisplayName = "Sauce Hot"),
    SauceWhite              UMETA(DisplayName = "Sauce White"),
    AlterMeat               UMETA(DisplayName = "Alter Meat"),
    AlterCookedMeat         UMETA(DisplayName = "Alter Cooked Meat"),
    AlterCheese             UMETA(DisplayName = "Alter Cheese"),
    BottomBun               UMETA(DisplayName = "Bottom Bun"),
    BottomBurnedBun         UMETA(DisplayName = "Bottom Burned Bun"),
    BottomCookedBun         UMETA(DisplayName = "Bottom Cooked Bun")
    // clang-format on
};

/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDBurgerPart : public ABDPickable
{
    GENERATED_BODY()

public:
    ABDBurgerPart();

    void ChangeType(TObjectPtr<UBDFoodPartDataAsset> Data);

    EFoodType PartType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sockets)
    TObjectPtr<USceneComponent> FoodSocket;

protected:
    virtual void BeginPlay() override;
};
