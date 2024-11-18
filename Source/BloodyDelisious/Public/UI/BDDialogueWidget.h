// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDDialogueWidget.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API UBDDialogueWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetText(FText InText);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DialogueText;
};
