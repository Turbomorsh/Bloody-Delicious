// Bloody Delisious. All Rights Reserved.

#include "Framework/BDGameMode.h"
#include "Player/BDPlayerCharacter.h"
#include "Player/BDPlayerController.h"
#include "UI/BDGameHUD.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
// for test visibility manager
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

    SetGameState(EBDGameState::GameInProgress);

    // for test visibility manager
    VisibilityManager = NewObject<UBDVisibilityManager>(this);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), ScaryTag, TargetActors);

    SpawnCustomers();
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

UClass* ABDGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AICustomerPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

AActor* ABDGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    if (!Player) return Super::ChoosePlayerStart_Implementation(Player);

    FName TargetTag = Player->IsA<AAIController>() ? ToNPCStartTag : Player->IsA<APlayerController>() ? ToPlayerStartTag : NAME_None;

    if (TargetTag == NAME_None) return Super::ChoosePlayerStart_Implementation(Player);

    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* PlayerStart = *It;
        UE_LOG(LogBDGameMode, Display, TEXT("%s | named: %s"), *PlayerStart->PlayerStartTag.ToString(), *PlayerStart->GetName());

        if (PlayerStart->PlayerStartTag == TargetTag) return PlayerStart;
    }
    return Super::ChoosePlayerStart_Implementation(Player);
}

void ABDGameMode::SpawnCustomers()
{
    if (!GetWorld()) return;

    for (uint32 i = 0; i < CustomerCount; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto BDAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(BDAIController);
    }
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
