// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Interactibles/BDStaticInteractionActor.h"
#include "BDFridgeDoor.generated.h"

/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDFridgeDoor : public ABDStaticInteractionActor
{
    GENERATED_BODY()

public:
    ABDFridgeDoor();

    virtual void Tick(float DeltaTime) override;

    virtual void Interact(TObjectPtr<UObject> Object) override;

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
    TObjectPtr<USceneComponent> DoorSocket;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY()
    FTimeline Timeline;

    UPROPERTY()
    TObjectPtr<UCurveFloat> Curve;

    FRotator InitRotator;

    bool IsOpen = false;

    virtual void BeginPlay() override;

    void BIndTimeline();

    UFUNCTION()
    void TimelineProgress(float Alpha);
};
