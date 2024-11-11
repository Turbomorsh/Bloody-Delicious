// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Framework/BDCoreTypes.h"
#include "BDGameMode.generated.h"

class UBDVisibilityManager;  // for test visibility manager

UCLASS()
class BLOODYDELISIOUS_API ABDGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABDGameMode();

    FOnGameStateChangedSignature OnGameStateChanged;

    UPROPERTY()
    UBDVisibilityManager* VisibilityManager;  // for test visibility manager

    virtual void StartPlay() override;
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    UFUNCTION(BlueprintCallable)
    void VisibilitiManagerLog();  // for test visibility manager

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditDefaultsOnly)
    FName ScarryTag = "Scarry";  // for test visibility manager

private:
    EBDGameState GameState = EBDGameState::Waiting;

    TArray<AActor*> TargetActors;  // for test visibility manager

    void SetGameState(EBDGameState State);
    void GameOver();
    void GameComplete();
};
