// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDPauseWidget.generated.h"

class UBDButton;

UCLASS()
class BLOODYDELISIOUS_API UBDPauseWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBDButton> ResumeButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBDButton> ResetButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBDButton> MainMenuButton;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> MenuLevel;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnResume();

    UFUNCTION()
    void OnReset();

    UFUNCTION()
    void OnMainMenu();
};
