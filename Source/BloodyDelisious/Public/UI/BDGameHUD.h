// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Framework/BDCoreTypes.h"
#include "BDGameHUD.generated.h"

class UBDGameplayWidget;
class UBDGameOverWidget;
class UBDPauseWidget;
class UBDGameCompletedWidget;

UCLASS()
class BLOODYDELISIOUS_API ABDGameHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDGameplayWidget> GameplayWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDGameOverWidget> GameOverWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDPauseWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDGameCompletedWidget> GameCompletedWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EBDGameState, TObjectPtr<UUserWidget>> GameWidgets;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;

    void OnGameStateChanged(EBDGameState State);
};
