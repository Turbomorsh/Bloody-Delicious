// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDPhraseWidget.generated.h"

class UTextBlock;

UCLASS()
class BLOODYDELISIOUS_API UBDPhraseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPhrase(FText InPhrase, float InDuration);

    UFUNCTION(BlueprintCallable)
    void SetPhraseWrapEditable(FText InPhrase, float InDuration, float InWrap);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Phrase_TextBlock;

    void HideWidget();
};
