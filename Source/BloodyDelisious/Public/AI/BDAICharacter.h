// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Framework/BDCoreTypes.h"
#include "Interactibles/BDFoodTray.h"
#include "Interactibles/BDInteract.h"
#include "BDAICharacter.generated.h"

class UBDBurgerTypeDataAsset;
class UBDDialogueWidget;
class UBDInteractionHintWidget;
class UBDGameplayWidget;
class UBehaviorTree;
class UBDHorrorManager;
class UUserWidget;
class UBDOrderWidget;

UCLASS() class BLOODYDELISIOUS_API ABDAICharacter : public ACharacter, public IBDInteract
{
    GENERATED_BODY()

public:
    ABDAICharacter();

    virtual void Interact(TObjectPtr<UObject> Object) override;
    virtual void Show() override;
    virtual void Hide() override;

    void MakeOrder();
    void OnOutside();
    void SetCustomerState(EBDCustomerStates NewState);

    void SetOrderData(TObjectPtr<UBDBurgerTypeDataAsset> InOrder);

    UFUNCTION(BlueprintCallable)
    EBDCustomerStates GetCustomerState() const { return CustomerState; };

    FOnCustomerOutsideSignature OnCustomerOutside;
    FOnCustomerStateChangedSignature OnCustomerStateChanged;
    FOnCustomerTimerChangedSignature OnCustomerTimerChanged;
    FOnCustomerTextSaySignature OnCustomerPhraseSay;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TObjectPtr<UBehaviorTree> BehsaviorTreeAsset;

    void Scream();

    UFUNCTION(BlueprintCallable)
    bool IsWaiting() const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (ClampMin = "0", ClampMax = "600"))
    float MaxSpeed = 212.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName CustomerStatusKeyName = "CustomerStatus";

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "600"))
    float TimeToEat = 10.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "120"))
    float TimeToPendingOrder = 5.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "120"))
    float TimeToCooking = 15.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "1"))
    float TimerUpdateInterval = 0.1f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AI")
    FName OrderTag{"Order display"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ExposeOnSpawn))
    TArray<FText> Dialogue;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
    TObjectPtr<USoundBase> ScreamSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> TraySocket;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
    TArray<TObjectPtr<UBDBurgerTypeDataAsset>> OrderTypes;
    TObjectPtr<UBDBurgerTypeDataAsset> OrderType;

    // @TODO: move to order structure
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI",
        meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Number of points for order fulfillment"))
    int32 Score = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hint")
    TSubclassOf<UBDInteractionHintWidget> HintWidgetClass = nullptr;

    UPROPERTY()
    TObjectPtr<UBDInteractionHintWidget> Hint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dialogue")
    TSubclassOf<UBDDialogueWidget> DialogueWidgetClass = nullptr;

    UPROPERTY()
    TObjectPtr<UBDDialogueWidget> DialogueWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hint")
    FText HintText = FText::FromString("pick cube");

    FOrderStruct Order;
    int32 DialoguePage = 0;

    bool PlayDialogue(TArray<FText> Dialogue, int32 Page);

    void TryGetOrder(TObjectPtr<ABDFoodTray> InOrder);

    virtual void BeginPlay() override;

private:
    EBDCustomerStates CustomerState = EBDCustomerStates::None;

    UPROPERTY()
    TObjectPtr<ABDFoodTray> CurrentFood;

    TMap<EBDCustomerTimers, FCustomerTimerData> CustomerTimersMap;
    FTimerHandle EatTimerHandle;

    UPROPERTY()
    UBDHorrorManager* HorrorManagerPtr;

    UPROPERTY()
    TObjectPtr<UBDOrderWidget> OrderWidget;

    UBDGameplayWidget* GetGameplayWidget() const;
    UBDOrderWidget* GetOrderWidget() const;

    UBDHorrorManager* GetHorrorManager();

    void InitializeTimers();
    void StartCustomerTimer(EBDCustomerTimers InETimer);
    void UpdateProgressBar(EBDCustomerTimers InETimer, float TotalTime);
    void CustomerTimerEnd(EBDCustomerTimers InETimer);

    void SetBlackboardEnumData(FName KeyName, EBDCustomerStates& NewState);

    void PendingTimeOut();
    void CookingTimeOut();
    void EatingTimeOut();

    bool IsOrderCorrect();

    void Hungry();
    void Ordering();
    void OrderAccepted();
    void OrderReady();
    void Eating();
    void Leaving();

    FText GetOrderDescription(const TArray<TEnumAsByte<EFoodType>>& inBurger);
};
