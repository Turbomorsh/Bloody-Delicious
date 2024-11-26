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
    Cheese,
    Salad,
    Tomato,
    Cucumber,
    Onion,
    TopBun,
    TopBurnedBun,
    TopCookedBun,
    Meat,
    CookedMeat,
    BurnedMeat,
    SauceYellow,
    SauceRed,
    SauceHot,
    SauceWhite,
    AlterMeat,
    AlterCookedMeat,
    AlterCheese,
    BottomBun,
    BottomBurnedBun,
    BottomCookedBun
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

    virtual bool Scream(int32 HorrorValue) override;

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
