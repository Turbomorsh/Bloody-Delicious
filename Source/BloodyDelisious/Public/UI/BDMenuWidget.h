// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDMenuWidget.generated.h"

class UBDButton;

UCLASS()
class BLOODYDELISIOUS_API UBDMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBDButton> StartGameButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBDButton> CloseGameButton;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> GameLevel;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION(BlueprintCallable)
    void OnStartGame();

    UFUNCTION(BlueprintCallable)
    void OnCloseGame();
};
