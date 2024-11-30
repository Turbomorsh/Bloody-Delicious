// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Framework/BDCoreTypes.h"
#include "BDGameMode.generated.h"

class UBDHorrorManager;
class UBDVisibilityManager;  // visibility manager
class AAIController;
class ABDCassete;

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
    TObjectPtr<UBDHorrorManager> GetHorrorManager() { return HorrorManager; };

    UPROPERTY()
    UBDVisibilityManager* VisibilityManager;  // visibility manager

    virtual void StartPlay() override;
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Test")
    FName ScaryTag = "Scary";  // visibility manager

    // group
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> GroupAIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName ToPlayerStartTag{"Player"};

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName CasseteSpawnTag{"Cassete"};

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Horror Manager")
    TSubclassOf<UBDHorrorManager> HorrorManagerClass;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game")
    TSubclassOf<ABDCassete> CasseteToWin;

    UPROPERTY()
    TObjectPtr<UBDHorrorManager> HorrorManager;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> GoodLevel;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> BadLevel;

private:
    EBDGameState GameState = EBDGameState::Waiting;
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    void SetGameState(EBDGameState State);
    void GameOver();
    void GameComplete();
    bool IsNextRoundExist() { return CurrentRound + 1 <= GameData.RoundsNum; };
    bool IsLimitsOver();

    void SpawnGroupController();
    UFUNCTION()
    void StartRound();
    void HandleRoundTransition();
    void GameTimerUpdate();

    UFUNCTION()
    void ResetOnePlayer(AController* Controller);

    FTransform GetCasseteSpawnTransform();
    void SpawnCassete();
};
