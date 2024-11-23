// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDStaticInteractionActor.h"
#include "Interactibles/BDPickable.h"
#include "BDTray.generated.h"

class ABDBurgerPart;
class ABDPlayerCharacter;
class UBDFoodPartDataAsset;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDTray : public ABDStaticInteractionActor
{
    GENERATED_BODY()

public:
    ABDTray();

    virtual void Interact(TObjectPtr<UObject> Object) override;

    void ChangeTrayType(TObjectPtr<UBDFoodPartDataAsset> NewType);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TObjectPtr<UBDFoodPartDataAsset> FoodDataAsset;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
    TSubclassOf<ABDBurgerPart> FoodClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Usage)
    int32 MaxUsage = 5;

    int32 UsageLeft = 5;

    virtual void BeginPlay() override;

    void TakeItem(TObjectPtr<ABDPlayerCharacter> Player);
};
