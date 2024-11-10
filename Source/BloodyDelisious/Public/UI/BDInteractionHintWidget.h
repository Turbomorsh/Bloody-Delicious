// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDInteractionHintWidget.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API UBDInteractionHintWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HintText;

public:
    void SetText(FText Text);
};
