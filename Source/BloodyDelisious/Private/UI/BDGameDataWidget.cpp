// Bloody Delisious. All Rights Reserved.

#include "UI/BDGameDataWidget.h"
#include "Framework/BDCoreTypes.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Framework/BDGameMode.h"

void UBDGameDataWidget::NativeConstruct()
{
    Super::NativeConstruct();

    const auto GameMode = GetBGGameMode();
    GameMode->OnGameDataChanged.AddUObject(this, &ThisClass::UpdateWidgetData);
    UpdateWidgetData(GetRoundSecondsRemaining());
}

int32 UBDGameDataWidget::GetCurrentRoundNum() const
{
    const auto GameMode = GetBGGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 UBDGameDataWidget::GetTotalRoundNum() const
{
    const auto GameMode = GetBGGameMode();
    return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 UBDGameDataWidget::GetRoundSecondsRemaining() const
{
    const auto GameMode = GetBGGameMode();
    return GameMode ? GameMode->GetRuondSecondsRemaning() : 0;
}

void UBDGameDataWidget::UpdateWidgetData(int32 Time)
{
    TotalTime_TextBlock->SetText(FText::FromString(UKismetStringLibrary::TimeSecondsToString(Time).Mid(0, 5)));

    FString RoundInfo = "Day ";
    RoundInfo.Append(FString::FromInt(GetCurrentRoundNum()));
    RoundInfo.Append(" / ");
    RoundInfo.Append(FString::FromInt(GetTotalRoundNum()));

    RoundInfo_TextBlock->SetText(FText::FromString(RoundInfo));
}

ABDGameMode* UBDGameDataWidget::GetBGGameMode() const
{
    return GetWorld() ? Cast<ABDGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
}
