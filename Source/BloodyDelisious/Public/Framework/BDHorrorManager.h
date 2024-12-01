// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BDHorrorManager.generated.h"

class ABDManager;
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnOrderScoreChangedSignature, int32, int32, int32);

USTRUCT(BlueprintType)
struct FDelayTime
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Delay", meta = (ClampMin = "0", ClampMax = "10"))
    float Min = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Delay", meta = (ClampMin = "0", ClampMax = "10"))
    float Max = 5.0f;
};

class IBDHorrorInterface;
class UBDFoodPartDataAsset;

UCLASS(meta = (IsBlueprintBase = true))
class BLOODYDELISIOUS_API UBDHorrorManager : public UObject
{
    GENERATED_BODY()

public:
    UBDHorrorManager();

    void StartUpManager();

    FOnOrderScoreChangedSignature OnOrderScoreChanged;

    int32 GetHorrorScore() { return HorrorScore; };
    int32 GetHorrorLimit() { return HorrorLimit; };

    int32 GetFineScore() { return FineScore; };
    int32 GetFineLimit() { return FineLimit; };

    void InitializeHorrorActors();

    void KillPlayer();

protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Manager")
    TSubclassOf<ABDManager> ManagerClass;

    TObjectPtr<ABDManager> ManagerRef;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Horror")
    TMap<TSubclassOf<AActor>, int32> HorrorMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    int32 HorrorLimit = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    int32 FineLimit = 5;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Timers")
    float DelayTime = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Timers")
    FDelayTime ScreamTimer;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Timers")
    FDelayTime NormalTimer;

    UFUNCTION()
    void StartUpHorrorEvent(int32 NewHorrorScore);

    void OrderScoreChanged(int32 InHorrorScore, int32 InAntiHorrorScore, int32 InFineScore);

    void ActivateHorrorActor(AActor* HorrorActor, bool bWasActive, TMap<AActor*, bool>& ActorActivationStatus);

    void DeactivateHorrorActor(AActor* HorrorActor, bool bWasActive, TMap<AActor*, bool>& ActorActivationStatus);

    void CallManager();

    int32 HorrorScore = 0;
    int32 FineScore = 0;
    TMap<AActor*, int32> HorrorActors;
};