// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDGameOverWidget.generated.h"

class UButton;

UCLASS()
class BLOODYDELISIOUS_API UBDGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> BackToMenuButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> AgainButton;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> MenuLevel;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    TObjectPtr<USoundBase> GameOverSound;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnBackToMenu();

    UFUNCTION()
    void OnAgain();

    UFUNCTION()
    void OnVisibility(ESlateVisibility InVisibility);
};
