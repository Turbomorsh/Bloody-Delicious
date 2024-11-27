// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Framework/BDCoreTypes.h"
#include "BDGameMode.generated.h"

class UBDHorrorManager;
class UBDVisibilityManager;  // for test visibility manager
class AAIController;

UCLASS()
class BLOODYDELISIOUS_API ABDGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABDGameMode();

    FOnGameStateChangedSignature OnGameStateChanged;
    FOnGameDataChangedSignature OnGameDataChanged;
    FOnAllCustomerExitedSignature OnAllCustomerExited;
    FOnRoundEndSignature OnRoundEnd;
    FOnRoundStartSignature OnRoundStart;

    FGameData GetGameData() const { return GameData; }
    int32 GetCurrentRoundNum() const { return CurrentRound; }
    int32 GetRuondSecondsRemaning() const { return RoundCountDown; }
    TObjectPtr<UBDHorrorManager> GetHorrorManager() { return HorrorManagerReference; };

    UPROPERTY()
    UBDVisibilityManager* VisibilityManager;  // for test visibility manager

    virtual void StartPlay() override;
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Test")
    FName ScaryTag = "Scary";  // for test visibility manager

    // group
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> GroupAIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName ToPlayerStartTag{"Player"};

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Horror Manager")
    TSubclassOf<UBDHorrorManager> HorrorManagerClass;

    UPROPERTY()
    TObjectPtr<UBDHorrorManager> HorrorManagerReference;

private:
    EBDGameState GameState = EBDGameState::Waiting;
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    UPROPERTY()
    TArray<AActor*> TargetActors;  // for test visibility manager

    void SetGameState(EBDGameState State);
    void GameOver();
    void GameComplete();

    void SpawnGroupController();
    void StartRound();
    void HandleRoundTransition();
    void GameTimerUpdate();

    // void ResetPlayers();
    void ResetOnePlayer(AController* Controller);
};
