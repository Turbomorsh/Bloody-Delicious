// Bloody Delisious. All Rights Reserved.

#include "UI/BDPauseWidget.h"
// #include "Components/Button.h"
#include "UI/BDButton.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

void UBDPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ResumeButton);
    ResumeButton->OnClicked.AddDynamic(this, &ThisClass::OnResume);

    check(ResetButton);
    ResetButton->OnClicked.AddDynamic(this, &ThisClass::OnReset);

    check(MainMenuButton);
    MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMainMenu);
}

void UBDPauseWidget::OnResume()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}

void UBDPauseWidget::OnReset()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}

void UBDPauseWidget::OnMainMenu()
{
    if (!MenuLevel.IsNull())
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(MenuLevel.GetAssetName()));
    }
}
