// Bloody Delisious. All Rights Reserved.

#include "Framework/BDGameMode.h"
#include "Player/BDPlayerCharacter.h"
#include "Player/BDPlayerController.h"
#include "UI/BDGameHUD.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
// for test visibility manager
#include "Framework/BDHorrorManager.h"
#include "Framework/BDVisibilityManager.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDGameMode, All, All);

ABDGameMode::ABDGameMode()
{
    DefaultPawnClass = ABDPlayerCharacter::StaticClass();
    PlayerControllerClass = ABDPlayerController::StaticClass();
    HUDClass = ABDGameHUD::StaticClass();

    // for test visibility manager
    //  PrimaryActorTick.bCanEverTick = true;
}

void ABDGameMode::StartPlay()
{
    Super::StartPlay();

    CurrentRound = 1;
    SpawnGroupController();
    StartRound();

    OnGameDataChanged.Broadcast(RoundCountDown);

    OnAllCustomerExited.AddUObject(this, &ThisClass::HandleRoundTransition);

    // for test visibility manager
    VisibilityManager = NewObject<UBDVisibilityManager>(this);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), ScaryTag, TargetActors);

    if (HorrorManagerClass) HorrorManagerReference = NewObject<UBDHorrorManager>(this, HorrorManagerClass);
}

void ABDGameMode::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ThisClass::GameTimerUpdate, 1.0f, true);
    SetGameState(EBDGameState::GameInProgress);
    OnRoundStart.Broadcast();
}

void ABDGameMode::HandleRoundTransition()
{
    if (CurrentRound + 1 <= GameData.RoundsNum)
    {
        ++CurrentRound;
        ResetOnePlayer(GetWorld()->GetFirstPlayerController());
        StartRound();
    }
    else
    {
        GameOver();
    }
}

void ABDGameMode::GameTimerUpdate()
{
    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        // round transition
        SetGameState(EBDGameState::Waiting);
        OnRoundEnd.Broadcast();
    }
    OnGameDataChanged.Broadcast(GetRuondSecondsRemaning());
}

// void ABDGameMode::ResetPlayers()
//{
//     if (!GetWorld()) return;
//
//     for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
//     {
//         ResetOnePlayer(It->Get());
//     }
// }

void ABDGameMode::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
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
    UE_LOG(LogBDGameMode, Display, TEXT("==== GAME OVER ===="));
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetGameState(EBDGameState::GameOver);
}

void ABDGameMode::GameComplete()
{
    SetGameState(EBDGameState::GameCompleted);
}

AActor* ABDGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    if (!Player) return Super::ChoosePlayerStart_Implementation(Player);

    // FName TargetTag = Player->IsA<AAIController>() ? ToNPCStartTag : Player->IsA<APlayerController>() ? ToPlayerStartTag : NAME_None;
    FName TargetTag = Player->IsA<APlayerController>() ? ToPlayerStartTag : NAME_None;

    if (TargetTag == NAME_None) return Super::ChoosePlayerStart_Implementation(Player);

    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* PlayerStart = *It;
        UE_LOG(LogBDGameMode, Display, TEXT("%s | named: %s"), *PlayerStart->PlayerStartTag.ToString(), *PlayerStart->GetName());

        if (PlayerStart->PlayerStartTag == TargetTag) return PlayerStart;
    }
    return Super::ChoosePlayerStart_Implementation(Player);
}

void ABDGameMode::SpawnGroupController()
{
    if (!GetWorld()) return;

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const auto BDGroupAIController = GetWorld()->SpawnActor<AAIController>(GroupAIControllerClass, SpawnInfo);
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
