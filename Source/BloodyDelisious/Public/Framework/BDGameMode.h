// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Framework/BDCoreTypes.h"
#include "BDGameMode.generated.h"

UCLASS()
class BLOODYDELISIOUS_API ABDGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABDGameMode();

    FOnGameStateChangedSignature OnGameStateChanged;

    virtual void StartPlay() override;
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

private:
    EBDGameState GameState = EBDGameState::Waiting;

    void SetGameState(EBDGameState State);
    void GameOver();
    void GameComplete();
};
