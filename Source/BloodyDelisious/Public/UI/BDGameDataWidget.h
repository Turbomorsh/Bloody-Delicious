// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDGameDataWidget.generated.h"

class ABDGameMode;
class UTextBlock;

UCLASS()
class BLOODYDELISIOUS_API UBDGameDataWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void NativeConstruct() override;

    int32 GetCurrentRoundNum() const;

    int32 GetTotalRoundNum() const;

    int32 GetRoundSecondsRemaining() const;

    void UpdateWidgetData(int32 Time);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* TotalTime_TextBlock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* RoundInfo_TextBlock;

private:
    ABDGameMode* GetBGGameMode() const;
};
