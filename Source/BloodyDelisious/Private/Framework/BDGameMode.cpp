// Bloody Delisious. All Rights Reserved.

#include "Framework/BDGameMode.h"
#include "Player/BDPlayerCharacter.h"
#include "Player/BDPlayerController.h"
#include "UI/BDGameHUD.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Framework/BDHorrorManager.h"
#include "Framework/BDVisibilityManager.h"
#include "Kismet/GameplayStatics.h"
#include "Interactibles/BDCassete.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDGameMode, All, All);

ABDGameMode::ABDGameMode()
{
    DefaultPawnClass = ABDPlayerCharacter::StaticClass();
    PlayerControllerClass = ABDPlayerController::StaticClass();
    HUDClass = ABDGameHUD::StaticClass();
}

void ABDGameMode::StartPlay()
{
    Super::StartPlay();

    // visibility manager
    VisibilityManager = NewObject<UBDVisibilityManager>(this);

    // horror manager
    if (HorrorManagerClass)
    {
        HorrorManager = NewObject<UBDHorrorManager>(this, HorrorManagerClass);
        HorrorManager->StartUpManager();
    }

    CurrentRound = 1;
    SpawnGroupController();
    StartRound();

    if (HorrorManager)
    {
        HorrorManager->InitializeHorrorActors();
        UE_LOG(LogBDGameMode, Display, TEXT("HorrorManager init"));
    }

    OnGameDataChanged.Broadcast(RoundCountDown);

    OnAllCustomerExited.AddUObject(this, &ThisClass::HandleRoundTransition);
}

void ABDGameMode::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ThisClass::GameTimerUpdate, 1.0f, true);
    SetGameState(EBDGameState::GameInProgress);
    OnRoundStart.Broadcast();

    const auto Character = Cast<ABDPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (Character)
    {
        UE_LOG(LogBDGameMode, Display, TEXT("Has cassete: %s"), *LexToString(Character->GetHaveCassete()));
    }
}

void ABDGameMode::HandleRoundTransition()
{
    if (IsNextRoundExist() && !IsLimitsOver())
    {
        ++CurrentRound;

        // last round
        if (!IsNextRoundExist())
        {
            SpawnCassete();
        }

        SetGameState(EBDGameState::Waiting);
        FTimerHandle TimerHandle1;
        FTimerDelegate TimerDelegate1;
        TimerDelegate1.BindUFunction(this, "ResetOnePlayer", GetWorld()->GetFirstPlayerController());
        GetWorld()->GetTimerManager().SetTimer(TimerHandle1, TimerDelegate1, 5.f, false);

        FTimerHandle TimerHandle2;
        FTimerDelegate TimerDelegate2;
        TimerDelegate2.BindUFunction(this, "StartRound");
        GetWorld()->GetTimerManager().SetTimer(TimerHandle2, TimerDelegate2, 5.f, false);
    }
    else
    {
        // GameOver();
        HorrorManager->KillPlayer();
    }
}

void ABDGameMode::GameTimerUpdate()
{
    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        // round transition
        OnRoundEnd.Broadcast();
    }
    OnGameDataChanged.Broadcast(GetRuondSecondsRemaning());
}

bool ABDGameMode::IsLimitsOver()
{
    if (!HorrorManager) return false;
    // get screem scores
    int32 HScore = HorrorManager->GetHorrorScore();
    int32 HLimit = HorrorManager->GetHorrorLimit();
    int32 FScore = HorrorManager->GetFineScore();
    int32 FLimit = HorrorManager->GetFineLimit();

    bool IsHorrorLimitOver = HScore >= HLimit;
    bool IsFineLimitOver = FScore >= FLimit;

    UE_LOG(LogBDGameMode, Display, TEXT("HScore: %i, HLimit: %i, FScore: %i, FLimit: %i, IsLimitOver: %s"),  //
        HScore, HLimit, FScore, FLimit, *LexToString(IsHorrorLimitOver || IsFineLimitOver));

    return IsHorrorLimitOver || IsFineLimitOver;
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

    const auto Character = Cast<ABDPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (Character->GetHaveCassete() && !IsNextRoundExist())
    {
        // SetGameState(EBDGameState::GameCompleted);

        if (!GoodLevel.IsNull())
        {
            UGameplayStatics::OpenLevel(GetWorld(), FName(GoodLevel.GetAssetName()));
        }
    }
    else
    {
        // SetGameState(EBDGameState::GameOver);

        if (!BadLevel.IsNull())
        {
            UGameplayStatics::OpenLevel(GetWorld(), FName(BadLevel.GetAssetName()));
        }
    }
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

void ABDGameMode::SpawnCassete()
{
    if (!GetWorld()) return;

    FTransform SpwnTransform;
    SpwnTransform = GetCasseteSpawnTransform();

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    if (CasseteToWin)
    {
        GetWorld()->SpawnActor<ABDCassete>(CasseteToWin, SpwnTransform, SpawnInfo);
        UE_LOG(LogBDGameMode, Display, TEXT("Cassete was spawn in %s"), *SpwnTransform.ToString());
    }
    else
    {
        UE_LOG(LogBDGameMode, Warning, TEXT("Cassete class not setup"));
    }
}

FTransform ABDGameMode::GetCasseteSpawnTransform()
{
    if (!GetWorld()) return FTransform::Identity;

    AActor* CasseteTargetSpawnActor = nullptr;
    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        if (It->ActorHasTag(CasseteSpawnTag))
        {
            CasseteTargetSpawnActor = *It;
            break;
        }
        else
        {
            UE_LOG(LogBDGameMode, Warning, TEXT("No actor with tag %s"), *CasseteSpawnTag.ToString());
        }
    }

    return CasseteTargetSpawnActor ? CasseteTargetSpawnActor->GetActorTransform() : FTransform::Identity;
}