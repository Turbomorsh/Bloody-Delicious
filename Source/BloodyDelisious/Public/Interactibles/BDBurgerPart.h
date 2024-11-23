// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/BDPickable.h"
#include "BDBurgerPart.generated.h"

class UBDFoodPartDataAsset;
UENUM(BlueprintType)
enum EFoodType : uint8
{
    Cheese,
    Salad,
    Tomato,
    Cucumber,
    Onion,
    TopBun,
    TopBurnedBun,
    TopCookedBun,
    Meet,
    CookedMeet,
    BurnedMeet,
    SauceYellow,
    SauceRed,
    SauceHot,
    SauceWhite,
    AlterMeet,
    AlterCookedMeet,
    AlterCheese,
    BottomBun,
    BottomBurnedBun,
    BottomCookedBun
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
