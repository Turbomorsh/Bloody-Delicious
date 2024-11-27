// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/BDHorrorInterface.h"
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
    Cucumber                UMETA(DisplayName = "Pickles"),
    Onion                   UMETA(DisplayName = "Onion"),
    TopBun                  UMETA(DisplayName = "Top Bun"),
    TopBurnedBun            UMETA(DisplayName = "Top Burned Bun"),
    TopCookedBun            UMETA(DisplayName = "Top Cooked Bun"),
    Meat                    UMETA(DisplayName = "Patty"),
    CookedMeat              UMETA(DisplayName = "Cooked Patty"),
    BurnedMeat              UMETA(DisplayName = "Burned Patty"),
    SauceYellow             UMETA(DisplayName = "Mustard"),
    SauceRed                UMETA(DisplayName = "Ketchup"),
    SauceHot                UMETA(DisplayName = "Chili sauce"),
    SauceWhite              UMETA(DisplayName = "Mayonnaise"),
    AlterMeat               UMETA(DisplayName = "Distorted Patty"),
    AlterCookedMeat         UMETA(DisplayName = "Distorted Cooked Patty"),
    AlterCheese             UMETA(DisplayName = "Distorted Cheese"),
    BottomBun               UMETA(DisplayName = "Bottom Bun"),
    BottomBurnedBun         UMETA(DisplayName = "Bottom Burned Bun"),
    BottomCookedBun         UMETA(DisplayName = "Bottom Cooked Bun")
    // clang-format on
};

/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDBurgerPart : public ABDPickable, public IBDHorrorInterface
{
    GENERATED_BODY()

public:
    ABDBurgerPart();

    void ChangeType(TObjectPtr<UBDFoodPartDataAsset> Data);

    virtual void Scream() override;

    virtual void DisableScream() override;

    EFoodType PartType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sockets)
    TObjectPtr<USceneComponent> FoodSocket;

protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Horror)
    int32 AlterTransformCost = 10;

    TObjectPtr<UBDFoodPartDataAsset> AlterTransformData;

    void AlterTransform();

    virtual void BeginPlay() override;
};
