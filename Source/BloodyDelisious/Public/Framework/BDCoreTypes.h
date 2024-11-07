#pragma once

#include "BDCoreTypes.generated.h"

UENUM()
enum class EBDGameState : uint8
{
    Waiting = 0,
    GameInProgress,
    GamePause,
    GameOver,
    GameCompleted
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EBDGameState);
