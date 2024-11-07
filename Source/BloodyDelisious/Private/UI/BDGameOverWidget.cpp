// Bloody Delisious. All Rights Reserved.

#include "UI/BDGameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UBDGameOverWidget::NativeOnInitialized()
{
    check(BackToMenuButton);
    BackToMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnBackToMenu);

    check(AgainButton);
    AgainButton->OnClicked.AddDynamic(this, &ThisClass::OnAgain);

    OnVisibilityChanged.AddDynamic(this, &ThisClass::OnVisibility);
}

void UBDGameOverWidget::OnBackToMenu()
{
    if (!MenuLevel.IsNull())
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(MenuLevel.GetAssetName()));
    }
}

void UBDGameOverWidget::OnAgain()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}

void UBDGameOverWidget::OnVisibility(ESlateVisibility InVisibility)
{
    if (InVisibility == ESlateVisibility::Visible)
    {
        PlaySound(GameOverSound);
    }
}
