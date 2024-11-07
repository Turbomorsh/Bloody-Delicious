// Bloody Delisious. All Rights Reserved.

#include "UI/BDMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UBDMenuWidget::NativeOnInitialized()
{

    Super::NativeOnInitialized();

    check(StartGameButton);
    StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);

    check(CloseGameButton);
    CloseGameButton->OnClicked.AddDynamic(this, &ThisClass::OnCloseGame);
}

void UBDMenuWidget::OnStartGame()
{
    if (!GameLevel.IsNull())
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(GameLevel.GetAssetName()));
    }
}

void UBDMenuWidget::OnCloseGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
