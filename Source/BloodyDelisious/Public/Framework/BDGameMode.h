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
    UPROPERTY(EditDefaultsOnly)
    FName ScaryTag = "Scary";  // for test visibility manager

    UPROPERTY(EditDefaultsOnly, Category = "Game | AI")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game | AI")
    TSubclassOf<APawn> AICustomerPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game | AI", meta = (ClampMin = "1", ClampMax = "50"))
    uint32 CustomerCount = 1;

    UPROPERTY(EditDefaultsOnly, Category = "Game | AI")
    FName ToNPCStartTag{"NPC"};
    UPROPERTY(EditDefaultsOnly, Category = "Game | AI")
    FName ToPlayerStartTag{"Player"};

private:
    EBDGameState GameState = EBDGameState::Waiting;

    TArray<AActor*> TargetActors;  // for test visibility manager

    void SetGameState(EBDGameState State);
    void GameOver();
    void GameComplete();

    void SpawnCustomers();
};
