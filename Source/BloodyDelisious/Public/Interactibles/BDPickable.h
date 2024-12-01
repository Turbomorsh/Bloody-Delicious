// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDInteract.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "BDPickable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabbed);

class ABDPlayerCharacter;
class UBDInteractionHintWidget;
UCLASS()
class BLOODYDELISIOUS_API ABDPickable : public AActor, public IBDInteract
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDPickable();

    void Interact(TObjectPtr<UObject> Object) override;

    void Show(TObjectPtr<UObject> InObject) override;

    void Hide() override;

    FGrabbed OnGrabbed;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void BindTimeLine();

    UFUNCTION()
    void TimelineProgress(float Alpha);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Note)
    TSubclassOf<UBDInteractionHintWidget> HintWidgetClass = nullptr;

    UPROPERTY()
    TObjectPtr<UBDInteractionHintWidget> Hint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Note)
    FText HintText = FText::FromString("pick cube");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Note)
    FText AlterHintText = FText::FromString("");

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Note)
    TSubclassOf<AActor> AnotherHintTrigger = nullptr;

    UPROPERTY()
    FTimeline PickableTimeline;

    UPROPERTY()
    TObjectPtr<UCurveFloat> Curve;

    UPROPERTY()
    TObjectPtr<USceneComponent> GrabLocationSocket;

    FVector InitGrabLocation;

    UFUNCTION()
    void Destruct();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void Drop(FRotator CameraRotation, FVector CameraLocation);

    void Grab(USceneComponent* Socket);
};
