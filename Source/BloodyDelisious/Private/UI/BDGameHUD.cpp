// Bloody Delisious. All Rights Reserved.

#include "UI/BDGameHUD.h"
#include "UI/BDGameplayWidget.h"
#include "UI/BDPauseWidget.h"
#include "UI/BDGameCompletedWidget.h"
#include "UI/BDGameOverWidget.h"
#include "Framework/BDGameMode.h"
#include "Framework/BDUtils.h"

void ABDGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(EBDGameState::GameInProgress, CreateWidget<UBDGameplayWidget>(GetWorld(), GameplayWidgetClass));
    GameWidgets.Add(EBDGameState::GamePause, CreateWidget<UBDPauseWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(EBDGameState::GameCompleted, CreateWidget<UBDGameCompletedWidget>(GetWorld(), GameCompletedWidgetClass));
    GameWidgets.Add(EBDGameState::GameOver, CreateWidget<UBDGameOverWidget>(GetWorld(), GameOverWidgetClass));

    for (auto& [UIState, GameWidget] : GameWidgets)
    {
        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ABDGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
        }
    }
}

void ABDGameHUD::OnGameStateChanged(EBDGameState State)
{
    if (CurrentWidget && State != EBDGameState::Waiting)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }
    bool bShowCursor = State != EBDGameState::GameInProgress && State != EBDGameState::Waiting;
    WorldUtils::SetUIInput(GetWorld(), bShowCursor);

    RoundTransition(State == EBDGameState::Waiting);
}

void ABDGameHUD::RoundTransition(bool InbShow)
{
    auto GamePlayWidget = Cast<UBDGameplayWidget>(CurrentWidget);
    if (GamePlayWidget)
    {
        GamePlayWidget->ShowTransitionInfo(InbShow);
    }
}