// Bloody Delisious. All Rights Reserved.

#include "UI/BDInteractionHintWidget.h"

#include "Components/TextBlock.h"

void UBDInteractionHintWidget::SetText(FText Text)
{
    if (HintText) HintText->SetText(Text);
}