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

UENUM(BlueprintType)
enum class EBDCustomerStates : uint8
{
    // clang-format off
    Hungry = 0     UMETA(DisplayName = "Hungry"),
    Ordering       UMETA(DisplayName = "Ordering"),
    OrderAccepted  UMETA(DisplayName = "Order Accepted"),
    OrderReady     UMETA(DisplayName = "Order Ready"),
    Eating         UMETA(DisplayName = "Eating"),
    Leaving        UMETA(DisplayName = "Leaving")
    // clang-format on
};

UENUM()
enum class EBDOrderStates : uint8
{
    Pending = 0,
    Cooking,
    Ready,
    NextInLine
};
