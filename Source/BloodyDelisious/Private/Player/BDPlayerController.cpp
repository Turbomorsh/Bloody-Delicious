// Bloody Delisious. All Rights Reserved.

#include "Player/BDPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/BDUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDPlayerController, All, All);

void ABDPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent) return;

    auto* Input = Cast<UEnhancedInputComponent>(InputComponent);
    check(Input);

    if (PauseAction)
    {
        PauseAction->bTriggerWhenPaused = true;
        Input->BindAction(PauseAction, ETriggerEvent::Started, this, &ThisClass::OnPauseGame);
    }
    else
    {
        UE_LOG(LogBDPlayerController, Error, TEXT("PauseAction isn't set!"));
    }
}

void ABDPlayerController::OnPauseGame()
{
    SetPause(!IsPaused());
}
