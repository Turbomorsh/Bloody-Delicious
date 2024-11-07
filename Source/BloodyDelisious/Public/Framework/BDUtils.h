#pragma once

#include "CoreMinimal.h"

class WorldUtils
{
public:
    static void SetUIInput(UWorld* World, bool Enabled)
    {
        if (!World) return;

        if (auto* PC = World->GetFirstPlayerController())
        {
            PC->SetShowMouseCursor(Enabled);
            Enabled ? PC->SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false)) : PC->SetInputMode(FInputModeGameOnly());
        }
    }
};