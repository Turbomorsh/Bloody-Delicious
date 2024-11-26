// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDHorrorManager.generated.h"

class IBDHorrorInterface;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSubmissionScoreChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnResistansScoreChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnOrderScoreChangedSignature, int32, int32, int32);

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

    FOnOrderScoreChangedSignature OnOrderScoreChanged;

    int32 GetHorrorScore() { return HorrorScore; };
    int32 GetHorrorLimit() { return HorrorLimit; };

    int32 GetFineScore() { return FineScore; };
    int32 GetFineLimit() { return FineLimit; };

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    int32 HorrorLimit = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    int32 FineLimit = 5;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Timers")
    float DelayTime = 1.f;

    void OrderScoreChanged(int32 InHorrorScore, int32 InAntiHorrorScore, int32 InFineScore);

private:
    int32 HorrorScore = 0;
    int32 FineScore = 0;

    int32 SubmissionScore = 0;
    int32 ResistansScore = 0;
};
