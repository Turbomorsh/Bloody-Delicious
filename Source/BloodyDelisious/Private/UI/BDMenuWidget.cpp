// Bloody Delisious. All Rights Reserved.

#include "UI/BDMenuWidget.h"
#include "UI/BDButton.h"
#include "Kismet/GameplayStatics.h"

void UBDMenuWidget::NativeOnInitialized()
{

    Super::NativeOnInitialized();

    check(StartGameButton);
    StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);

    check(CloseGameButton);
    CloseGameButton->OnClicked.AddDynamic(this, &ThisClass::OnCloseGame);
}

void UBDMenuWidget::OnStartGame_Implementation() {}

void UBDMenuWidget::OnCloseGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
