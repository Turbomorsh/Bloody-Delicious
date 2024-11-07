// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BDMenuHUD.generated.h"

class UBDMenuWidget;

UCLASS()
class BLOODYDELISIOUS_API ABDMenuHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBDMenuWidget> MenuWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<UBDMenuWidget> MenuWidget;
};
