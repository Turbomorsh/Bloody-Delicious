// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDHorrorManager.generated.h"

class IBDHorrorInterface;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSubmissionScoreChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnResistansScoreChangedSignature, int32);

class UBDFoodPartDataAsset;
UCLASS(meta = (IsBlueprintBase = true))
class BLOODYDELISIOUS_API UBDHorrorManager : public UObject
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    UBDHorrorManager();

    FOnSubmissionScoreChangedSignature OnSubmissionScoreChanged;
    FOnResistansScoreChangedSignature OnResistansScoreChanged;

    UFUNCTION()
    void AddScore(int InScore);

    UFUNCTION()
    void RemoveScore(int InScore);

    void OnChangedSubmissionScore(int32 InScore);
    void OnChangedResistansScore(int32 InScore);

protected:
    // Called when the game starts or when spawned

    UFUNCTION()
    void StartUpHorrorEvent();

    void PlayHorrorEvent(IBDHorrorInterface* InterfaceActor);

    UFUNCTION()
    void DisableHorrorEvent(TArray<AActor*> HorroredActors);

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Horror)
    TMap<TSubclassOf<AActor>, int32> HorrorMap;

    int HorrorScore = 0;

    int HorrorLimit = 5;

    int32 SubmissionScore = 0;
    int32 ResistansScore = 0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Timers)
    float ScreamDelayTime = 1.f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Timers)
    float NormalDelayTime = 1.f;
};
