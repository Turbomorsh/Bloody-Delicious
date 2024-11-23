// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/BDCoreTypes.h"
#include "BDGameplayWidget.generated.h"

class UProgressBar;
class UTextBlock;
class ABDAICharacter;

UCLASS()
class BLOODYDELISIOUS_API UBDGameplayWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void NativeConstruct() override;

    void SubscribeToNPCTimers(ABDAICharacter* NPC);
    void UnSubscribeFromNPCTimers(ABDAICharacter* NPC);

    void SubscribeToNPCPhrases(ABDAICharacter* NPC);
    void UnSubscribeToNPCPhrases(ABDAICharacter* NPC);

    void OnTimerChanged(float TimeRemaning, EBDCustomerTimers InETimer);

    void OnPhraseSay(FText InPhrase, bool bIsActive);

    void ShowTransitionInfo(bool bShow);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
    UProgressBar* TimerProgressBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor PendingColor = FLinearColor::Blue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor CookingColor = FLinearColor::Red;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* Phrase_TextBlock;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* Transition_TextBlock;

private:
    ABDAICharacter* CurrentNPC;
    FDelegateHandle CustomerTimerChangedHandle;
    FDelegateHandle CustomerPhraseHandle;
};
