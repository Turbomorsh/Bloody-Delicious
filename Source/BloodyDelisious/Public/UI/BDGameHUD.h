// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Framework/BDCoreTypes.h"
#include "BDGameHUD.generated.h"

class UBDDayDelimiterWidget;
class UBDGameplayWidget;
class UBDGameOverWidget;
class UBDPauseWidget;
class UBDGameCompletedWidget;

UCLASS()
class BLOODYDELISIOUS_API ABDGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    UUserWidget* GetGameplayWidget() const { return GameWidgets[EBDGameState::GameInProgress]; };

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDGameplayWidget> GameplayWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDGameOverWidget> GameOverWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDPauseWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDGameCompletedWidget> GameCompletedWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDDayDelimiterWidget> DayDelimiterClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EBDGameState, TObjectPtr<UUserWidget>> GameWidgets;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;

    void OnGameStateChanged(EBDGameState State);
    void RoundTransition(bool InbShow);
};
