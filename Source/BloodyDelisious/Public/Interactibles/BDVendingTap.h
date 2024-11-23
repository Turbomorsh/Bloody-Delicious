// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDCup.h"
#include "Components/TimelineComponent.h"
#include "Interactibles/BDStaticInteractionActor.h"
#include "BDVendingTap.generated.h"

class ABDCup;
class UBoxComponent;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDVendingTap : public ABDStaticInteractionActor
{
    GENERATED_BODY()

public:
    ABDVendingTap();

    virtual void Interact(TObjectPtr<UObject> Object) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Horror)
    bool HorrorMode = false;

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Component)
    TObjectPtr<UStaticMeshComponent> TapMesh;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Component)
    TObjectPtr<USceneComponent> CupSocket;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Component)
    TObjectPtr<UBoxComponent> TapCollision;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Component)
    TObjectPtr<USceneComponent> RootScene;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Component)
    TObjectPtr<USceneComponent> TapSocket;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = States)
    TEnumAsByte<EDrinkType> DrinkType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sounds)
    TObjectPtr<USoundBase> ScreamSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sounds)
    TObjectPtr<USoundBase> PourSound;

    UPROPERTY()
    TObjectPtr<ABDCup> CupReference;

    UPROPERTY()
    FTimeline Timeline;

    UPROPERTY()
    TObjectPtr<UCurveFloat> Curve;

    FRotator InitialRotator;

    virtual void BeginPlay() override;

    void BindTimeLine();

    UFUNCTION()
    void TimeLineUpdate(float Alpha);

    void TakeCup(TObjectPtr<ABDCup> InCup);

    UFUNCTION()
    void ClearCup();
};
