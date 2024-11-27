// Bloody Delisious. All Rights Reserved.

#include "UI/BDGameplayWidget.h"
#include "AI/BDAICharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDGameplayWidget, All, All);

void UBDGameplayWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UBDGameplayWidget::SubscribeToNPCTimers(ABDAICharacter* NPC)
{
    if (NPC)
    {
        CurrentNPC = NPC;
        CustomerTimerChangedHandle = CurrentNPC->OnCustomerTimerChanged.AddUObject(this, &ThisClass::OnTimerChanged);
        TimerProgressBar->SetVisibility(ESlateVisibility::Visible);
    }
}

void UBDGameplayWidget::UnSubscribeFromNPCTimers(ABDAICharacter* NPC)
{
    if (NPC && CustomerTimerChangedHandle.IsValid())
    {
        NPC->OnCustomerTimerChanged.Remove(CustomerTimerChangedHandle);
        TimerProgressBar->SetVisibility(ESlateVisibility::Collapsed);
        CurrentNPC = nullptr;
    }
}

void UBDGameplayWidget::OnTimerChanged(float TimeRemaning, EBDCustomerTimers InETimer)
{
    switch (InETimer)
    {
        case EBDCustomerTimers::Pending:
            TimerProgressBar->SetFillColorAndOpacity(FLinearColor::Blue);
            break;
        case EBDCustomerTimers::Cooking:
            TimerProgressBar->SetFillColorAndOpacity(FLinearColor::Red);
            break;
        case EBDCustomerTimers::None:
            break;
        default:
            break;
    }

    TimerProgressBar->SetPercent(TimeRemaning);

    if (FMath::IsNearlyZero(TimeRemaning) || TimeRemaning < 0)
    {
        UnSubscribeFromNPCTimers(CurrentNPC);
    }
}

void UBDGameplayWidget::ShowTransitionInfo(bool bShow)
{
    UE_LOG(LogBDGameplayWidget, Display, TEXT("TransitionInfo"));
    if (bShow)
    {
        Transition_TextBlock->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        Transition_TextBlock->SetVisibility(ESlateVisibility::Collapsed);
    }
}
