// Bloody Delisious. All Rights Reserved.

#include "Framework/BDGameMode.h"
#include "Player/BDPlayerCharacter.h"
#include "Player/BDPlayerController.h"
#include "UI/BDGameHUD.h"
// for test visibility manager
#include "Framework/BDVisibilityManager.h"
#include "Kismet/GameplayStatics.h"

ABDGameMode::ABDGameMode()
{
    DefaultPawnClass = ABDPlayerCharacter::StaticClass();
    PlayerControllerClass = ABDPlayerController::StaticClass();
    HUDClass = ABDGameHUD::StaticClass();

    // for test visibility manager
    PrimaryActorTick.bCanEverTick = true;
}

void ABDGameMode::StartPlay()
{
    Super::StartPlay();

    SetGameState(EBDGameState::GameInProgress);

    // for test visibility manager
    VisibilityManager = NewObject<UBDVisibilityManager>(this);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), ScarryTag, TargetActors);
}

bool ABDGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        SetGameState(EBDGameState::GamePause);
    }

    return PauseSet;
}

bool ABDGameMode::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetGameState(EBDGameState::GameInProgress);
    }

    return PauseCleared;
}

void ABDGameMode::SetGameState(EBDGameState State)
{
    if (GameState == State) return;

    GameState = State;
    OnGameStateChanged.Broadcast(GameState);
}

void ABDGameMode::GameOver()
{
    SetGameState(EBDGameState::GameOver);
}

void ABDGameMode::GameComplete()
{
    SetGameState(EBDGameState::GameCompleted);
}

// for test visibility manager
void ABDGameMode::VisibilitiManagerLog()
{
    VisibilityManager->CameraLog();
}

void ABDGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    for (auto TargetActor : TargetActors)
    {
        VisibilityManager->IsActorVisible(TargetActor);
    }
}
