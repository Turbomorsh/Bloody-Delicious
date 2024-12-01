// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDInteract.h"
#include "Components/TimelineComponent.h"
#include "Framework/BDHorrorInterface.h"
#include "GameFramework/Actor.h"
#include "BDDoor.generated.h"

class UBDInteractionHintWidget;
class UTimelineComponent;

UENUM()
enum class EDoorStates : uint8
{
    Closed,
    Opened,
    Locked,
    SlightlyOpen
};

USTRUCT(BlueprintType)

struct FDoorData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
    TObjectPtr<USoundBase> Sound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds", meta = (ClampMin = "0"))
    float PlayRate = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Text")
    FString HintText{"Interact"};
};

UCLASS()
class BLOODYDELISIOUS_API ABDDoor : public AActor, public IBDInteract, public IBDHorrorInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDDoor();

    virtual void Interact(TObjectPtr<UObject> Object) override;
    virtual void Show(TObjectPtr<UObject> InObject) override;
    virtual void Hide() override;

    virtual void Scream() override;
    virtual void DisableScream() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> OutDoorMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
    TObjectPtr<USceneComponent> DoorSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Note)
    TSubclassOf<UBDInteractionHintWidget> HintWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
    TObjectPtr<USoundBase> ScreamSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door Settings")
    TMap<EDoorStates, FDoorData> DoorData;

    UPROPERTY()
    TObjectPtr<UBDInteractionHintWidget> Hint;

    UPROPERTY()
    FTimeline Timeline;

    UPROPERTY()
    TObjectPtr<UCurveFloat> Curve;

    EDoorStates DoorState = EDoorStates::Closed;

    FRotator InitRotator;

    void BIndTimeline();

    UFUNCTION()
    void TimelineProgress(float Alpha);

private:
    void ToggleOpenDoor();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void DoorCreak();
};
