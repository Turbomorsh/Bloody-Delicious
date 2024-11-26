// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDStaticInteractionActor.h"
#include "Interactibles/BDPickable.h"
#include "BDCassete.generated.h"

/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDCassete : public ABDStaticInteractionActor
{
    GENERATED_BODY()

public:
    ABDCassete();

    virtual void Interact(TObjectPtr<UObject> Object) override;

protected:
    void Grab(USceneComponent* Socket);

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> StaticMesh;
};
