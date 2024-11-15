// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/BDPickable.h"
#include "BDPlate.generated.h"

enum EFoodType : uint8;
class ABDBurgerPart;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDPlate : public ABDPickable
{
    GENERATED_BODY()

public:
    ABDPlate();

    void Interact(TObjectPtr<UObject> Object) override;

    TArray<EFoodType> GetBurger();

protected:
    virtual void BeginPlay() override;

    void PlaceItem(TObjectPtr<ABDBurgerPart> BurgerPart);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sockets)
    TObjectPtr<USceneComponent> FoodSocket;

    TArray<EFoodType> TypesArray;

    TObjectPtr<ABDBurgerPart> LastPart;
};
