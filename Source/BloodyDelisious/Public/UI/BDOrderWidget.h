// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/BDCoreTypes.h"
#include "BDOrderWidget.generated.h"

class UTextBlock;

UCLASS()
class BLOODYDELISIOUS_API UBDOrderWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void NativeConstruct() override;

    FOnOrderTextSignature OnOrderText;
    void HandleVisibility(EBDCustomerStates State);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* OrderTitle_TextBlock;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* OrderParts_TextBlock;

    void SetOrderText(FText InTitle, FText InDescription);
};
