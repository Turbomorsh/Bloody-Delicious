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

    CurrentRound = 1;
    StartRound();
    SpawnGroupController();

    // SpawnCustomers();
    SetGameState(EBDGameState::GameInProgress);
    OnGameDataChanged.Broadcast(RoundCountDown);

    // for test visibility manager
    VisibilityManager = NewObject<UBDVisibilityManager>(this);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), ScaryTag, TargetActors);
}

void ABDGameMode::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ThisClass::GameTimerUpdate, 1.0f, true);
}

void ABDGameMode::GameTimerUpdate()
{
    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
    OnGameDataChanged.Broadcast(GetRuondSecondsRemaning());
}

void ABDGameMode::ResetPlayers()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

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

UClass* ABDGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    // if (InController && InController->IsA<AAIController>())
    //{
    //     return AICustomerPawnClass;
    // }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
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

void ABDGameMode::SpawnCustomers()
{
    /*if (!GetWorld()) return;

    CustomerGroups.Empty();
    UE_LOG(LogTemp, Log, TEXT("SpawnCustomers: start creating AI character groups"));

    for (const TPair<TSubclassOf<APawn>, int32>& Pair : GameData.CustomersTypeNum)
    {
        AICustomerPawnClass = Pair.Key;
        int32 NumToSpawn = Pair.Value;

        TArray<AAIController*> NewGroup;
        UE_LOG(LogTemp, Log, TEXT("Creating a group for Pawn class: %s, amount: %d"), *AICustomerPawnClass->GetName(), NumToSpawn);
        for (int32 i = 0; i < NumToSpawn; ++i)
        {
            FActorSpawnParameters SpawnInfo;
            SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            const auto BDAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

            UE_LOG(LogTemp, Log, TEXT("AIController successfully created: %s"), *BDAIController->GetName());

            RestartPlayer(BDAIController);
            NewGroup.Add(BDAIController);

            APawn* ControlledPawn = BDAIController->GetPawn();
            if (ControlledPawn)
            {
                UE_LOG(LogTemp, Log, TEXT("Pawn successfully created: %s, Controller: %s"), *ControlledPawn->GetName(),
                    *BDAIController->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Error: Controller %s doesn't control Pawn!"), *BDAIController->GetName());
            }
        }
        CustomerGroups.Add(AICustomerPawnClass, NewGroup);
        UE_LOG(LogTemp, Log, TEXT("Group for %s created, amount Controllers: %d"), *AICustomerPawnClass->GetName(), NewGroup.Num());
    }

    UE_LOG(LogTemp, Log, TEXT("All groupd AI characters have been created, amount of types: %d"), CustomerGroups.Num());

    for (const auto& GroupPair : CustomerGroups)
    {
        const TSubclassOf<APawn>& PawnClass = GroupPair.Key;
        const TArray<AAIController*>& Controllers = GroupPair.Value;

        int32 NumControllers = Controllers.Num();
        FString PawnClassName = PawnClass ? PawnClass->GetName() : TEXT("Unknown");
        UE_LOG(LogTemp, Log, TEXT("Group for Pawn class: %s contains %d controllers"), *PawnClassName, NumControllers);

        for (const AAIController* Controller : Controllers)
        {
            if (Controller && Controller->GetPawn())
            {
                UE_LOG(LogTemp, Log, TEXT(" - Controller: %s, Pawn: %s"), *Controller->GetName(), *Controller->GetPawn()->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT(" - Controller: %s not tied to Pawn!"), *Controller->GetName());
            }
        }
    }*/
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
