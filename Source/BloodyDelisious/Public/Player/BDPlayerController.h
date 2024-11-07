// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BDPlayerController.generated.h"

class UInputAction;

UCLASS()
class BLOODYDELISIOUS_API ABDPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void OnPauseGame();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> PauseAction;

    virtual void SetupInputComponent() override;
};
