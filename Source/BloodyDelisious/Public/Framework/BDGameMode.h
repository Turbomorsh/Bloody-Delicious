// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Framework/BDCoreTypes.h"
#include "BDGameMode.generated.h"

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

    FGameData GetGameData() const { return GameData; }
    int32 GetCurrentRoundNum() const { return CurrentRound; }
    int32 GetRuondSecondsRemaning() const { return RoundCountDown; }

    UPROPERTY()
    UBDVisibilityManager* VisibilityManager;  // for test visibility manager

    virtual void StartPlay() override;
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    UFUNCTION(BlueprintCallable)
    void VisibilitiManagerLog();  // for test visibility manager

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Test")
    FName ScaryTag = "Scary";  // for test visibility manager

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    // UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AICustomerPawnClass = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName ToNPCStartTag{"NPC"};

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName ToPlayerStartTag{"Player"};

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

private:
    EBDGameState GameState = EBDGameState::Waiting;
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    TArray<AActor*> TargetActors;  // for test visibility manager

    void SetGameState(EBDGameState State);
    void GameOver();
    void GameComplete();

    void SpawnCustomers();
    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);
};
