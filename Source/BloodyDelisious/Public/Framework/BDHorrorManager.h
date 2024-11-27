// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BDHorrorManager.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnOrderScoreChangedSignature, int32, int32, int32);

class IBDHorrorInterface;
class UBDFoodPartDataAsset;

UCLASS(meta = (IsBlueprintBase = true))
class BLOODYDELISIOUS_API UBDHorrorManager : public UObject
{
    GENERATED_BODY()

public:
    UBDHorrorManager();

    FOnOrderScoreChangedSignature OnOrderScoreChanged;

    int32 GetHorrorScore() { return HorrorScore; };
    int32 GetHorrorLimit() { return HorrorLimit; };

    int32 GetFineScore() { return FineScore; };
    int32 GetFineLimit() { return FineLimit; };

    void InitializeHorrorActors();

protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Horror")
    TMap<TSubclassOf<AActor>, int32> HorrorMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    int32 HorrorLimit = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    int32 FineLimit = 5;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Timers")
    float DelayTime = 1.f;

    UFUNCTION()
    void StartUpHorrorEvent(int32 NewHorrorScore);

    void PlayHorrorEvent(IBDHorrorInterface* InterfaceActor);
    void OrderScoreChanged(int32 InHorrorScore, int32 InAntiHorrorScore, int32 InFineScore);

<<<<<<< Updated upstream
    UFUNCTION()
    void DisableHorrorEvent(TArray<AActor*> HorroredActors);

    int32 SubmissionScore = 0;
    int32 ResistansScore = 0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Timers)
    float ScreamDelayTime = 1.f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Timers)
    float NormalDelayTime = 1.f;

    == == == = private :
>>>>>>> Stashed changes
        int32 HorrorScore = 0;
    int32 FineScore = 0;
};
