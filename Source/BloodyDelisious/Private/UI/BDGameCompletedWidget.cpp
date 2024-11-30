// Bloody Delisious. All Rights Reserved.

#include "UI/BDGameCompletedWidget.h"
#include "UI/BDButton.h"
#include "Kismet/GameplayStatics.h"

void UBDGameCompletedWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(BackToMenuButton);
    BackToMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnBackToMenu);

    check(AgainButton);
    AgainButton->OnClicked.AddDynamic(this, &ThisClass::OnAgain);

    OnVisibilityChanged.AddDynamic(this, &ThisClass::OnVisibility);
}

void UBDGameCompletedWidget::OnBackToMenu()
{
    if (!MenuLevel.IsNull())
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(MenuLevel.GetAssetName()));
    }
}

void UBDGameCompletedWidget::OnAgain()
{
    // const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    // UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));

    if (!GameLevel.IsNull())
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(GameLevel.GetAssetName()));
    }
}

void UBDGameCompletedWidget::OnVisibility(ESlateVisibility InVisibility)
{
    if (InVisibility == ESlateVisibility::Visible)
    {
        PlaySound(GameCompleteSound);
    }
}
