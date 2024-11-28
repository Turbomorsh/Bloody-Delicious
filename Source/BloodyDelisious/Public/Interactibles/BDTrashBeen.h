// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/BDStaticInteractionActor.h"
#include "BDTrashBeen.generated.h"

class ABDFoodTray;
class ABDPlate;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDTrashBeen : public ABDStaticInteractionActor
{
    GENERATED_BODY()

public:
    ABDTrashBeen();

    virtual void Interact(TObjectPtr<UObject> Object) override;

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Sockets)
    TObjectPtr<USceneComponent> TrashSocket;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Sockets)
    TObjectPtr<UStaticMeshComponent> Mesh;

    void ClearPlate(TObjectPtr<ABDPlate> InPlate);

    void ClearTray(TObjectPtr<ABDFoodTray> InTray);
};
