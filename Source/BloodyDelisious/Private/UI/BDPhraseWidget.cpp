// Bloody Delisious. All Rights Reserved.

#include "UI/BDPhraseWidget.h"
#include "Components/TextBlock.h"
// #include "Animation/WidgetAnimation.h"
// #include "Animation/UMGSequencePlayer.h"

void UBDPhraseWidget::SetPhrase(FText InPhrase, float InDuration)
{
    Phrase_TextBlock->SetText(InPhrase);
    Phrase_TextBlock->SetAutoWrapText(true);
    Phrase_TextBlock->SetWrapTextAt(300.0f);
    SetVisibility(ESlateVisibility::Visible);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::HideWidget, InDuration, false);
}

void UBDPhraseWidget::HideWidget()
{
    SetVisibility(ESlateVisibility::Hidden);
}