// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDInteract.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "BDDoor.generated.h"

class UBDInteractionHintWidget;
class UTimelineComponent;

UENUM()
enum EDoorStates
{
    Closed,
    Opened,
    Locked,
    SlightlyOpen
};

UCLASS()
class BLOODYDELISIOUS_API ABDDoor : public AActor, public IBDInteract
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDDoor();

    virtual void Interact(TObjectPtr<UObject> Object) override;
    virtual void Show() override;
    virtual void Hide() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
    TObjectPtr<USceneComponent> DoorSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Note)
    TSubclassOf<UBDInteractionHintWidget> HintWidgetClass;

    UPROPERTY()
    TObjectPtr<UBDInteractionHintWidget> Hint;

    UPROPERTY()
    FTimeline Timeline;

    UPROPERTY()
    TObjectPtr<UCurveFloat> Curve;

    EDoorStates DoorState = EDoorStates::Closed;

    void BIndTimeline();

    UFUNCTION()
    void TimelineProgress(float Alpha);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
