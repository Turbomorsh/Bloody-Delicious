// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDDayDelimiterWidget.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API UBDDayDelimiterWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    TObjectPtr<UTextBlock> Text;
};
