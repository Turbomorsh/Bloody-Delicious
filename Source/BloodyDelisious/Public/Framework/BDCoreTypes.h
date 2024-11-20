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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameDataChangedSignature, int32);

// Timers
UENUM()
enum class EBDCustomerTimers : uint8
{
    Pending = 0,
    Cooking,
    None
};

USTRUCT(BlueprintType)
struct FCustomerTimerData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    EBDCustomerTimers Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float Duration;

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    FTimerDelegate OnMainTimerClearDelegate;

    FTimerHandle ProgressBarTimerHandle;
    FTimerDelegate ProgressBarTimerDelegate;

    float ProgressBarUpdateInterval;

    FCustomerTimerData() : Type(EBDCustomerTimers::None), Duration(0.0f), ProgressBarUpdateInterval(0.1f) {}
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCustomerTimerChangedSignature, float, EBDCustomerTimers);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCustomerTextSaySignature, FText, bool);

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

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game",  //
        meta = (ClampMin = "1", ClampMax = "20", ToolTip = "Costomers count"))
    int32 CustomersNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game",  //
        meta = (ClampMin = "1", ClampMax = "10", ToolTip = "Days count"))
    int32 RoundsNum = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game",  //
        meta = (ClampMin = "3", ClampMax = "300", ToolTip = "Day time in sec"))
    int32 RoundTime = 10;  // in seconds;
};