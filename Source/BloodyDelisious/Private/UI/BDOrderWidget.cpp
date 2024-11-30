// Bloody Delisious. All Rights Reserved.

#include "UI/BDOrderWidget.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDOrderWidget, All, All);

void UBDOrderWidget::NativeConstruct()
{
    Super::NativeConstruct();

    OnOrderText.AddUObject(this, &ThisClass::SetOrderText);
}

void UBDOrderWidget::SetOrderText(FText InTitle, FText InDescription)
{
    OrderTitle_TextBlock->SetText(InTitle);
    OrderParts_TextBlock->SetText(InDescription);
}

void UBDOrderWidget::HandleVisibility(EBDCustomerStates State)
{
    if (State == EBDCustomerStates::OrderAccepted)
    {
        if (GetVisibility() != ESlateVisibility::Visible)
        {
            SetVisibility(ESlateVisibility::Visible);
        }
    }
    else if (State == EBDCustomerStates::Leaving || State == EBDCustomerStates::Eating)
    {
        if (GetVisibility() != ESlateVisibility::Collapsed)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Widget visibility updated based on state."));
}
