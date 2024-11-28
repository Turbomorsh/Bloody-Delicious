// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/BDStaticInteractionActor.h"
#include "BDPickableGiver.generated.h"

class ABDPlayerCharacter;
class UBoxComponent;
class ABDPickable;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDPickableGiver : public ABDStaticInteractionActor
{
    GENERATED_BODY()

public:
    ABDPickableGiver();

    virtual void Interact(TObjectPtr<UObject> Object) override;

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickable")
    TSubclassOf<ABDPickable> PickableClass;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Root")
    TObjectPtr<USceneComponent> RootScene;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Collision")
    TObjectPtr<UBoxComponent> Collision;

    void GiveActor(TObjectPtr<ABDPlayerCharacter> Player);
};
