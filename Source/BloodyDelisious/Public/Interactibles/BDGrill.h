// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/BDStaticInteractionActor.h"
#include "BDGrill.generated.h"

/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDGrill : public ABDStaticInteractionActor
{
    GENERATED_BODY()

public:
    ABDGrill();

    virtual void Interact(TObjectPtr<UObject> Object) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
    TObjectPtr<UStaticMeshComponent> Collision;
};
