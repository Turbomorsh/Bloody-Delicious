// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Framework/BDCoreTypes.h"
#include "Interactibles/BDFoodTray.h"
#include "Interactibles/BDInteract.h"
#include "BDAICharacter.generated.h"

class UBDDialogueWidget;
class UBDInteractionHintWidget;
class UBehaviorTree;
class ABDOrderManager;

UCLASS() class BLOODYDELISIOUS_API ABDAICharacter : public ACharacter, public IBDInteract
{
    GENERATED_BODY()

public:
    ABDAICharacter();

    virtual void Interact(TObjectPtr<UObject> Object) override;
    virtual void Show() override;
    virtual void Hide() override;

    void MakeOrder();
    void SetCustomerState(EBDCustomerStates NewState);

    UFUNCTION(BlueprintCallable)
    EBDCustomerStates GetCustomerState() const { return CustomerState; };

    // void SetOrderManagerPtr(TObjectPtr<ABDOrderManager> InOrderManager) { OrderManager = InOrderManager; };
    FOnCookTimerChangedSignature OnCookTimerChanged;
    FOnPendingTimerChangedSignature OnPendingTimerChanged;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TObjectPtr<UBehaviorTree> BehsaviorTreeAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Dialogue", meta = (ExposeOnSpawn))
    TArray<FText> Dialogue;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (ClampMin = "0", ClampMax = "600"))
    float MaxSpeed = 212.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName CustomerStatusKeyName = "CustomerStatus";

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "600"))
    float TimeHungryAgain = 10.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "600"))
    float TimeToEat = 10.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "120"))
    float TimeToPendingOrder = 5.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "120"))
    float TimeToCooking = 15.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "AI|Timers", meta = (ClampMin = "0", ClampMax = "1"))
    float TimeUpdateInterval = 0.1f;

    void PlayDialogue(TArray<FText> Dialogue, int Page);

    void TryGetOrder(TObjectPtr<ABDFoodTray> InOrder);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
    TObjectPtr<USceneComponent> TraySocket;

    FOrderStruct Order;

    // bool Talked = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Hint)
    TSubclassOf<UBDInteractionHintWidget> HintWidgetClass = nullptr;

    UPROPERTY()
    TObjectPtr<UBDInteractionHintWidget> Hint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Dialogue)
    TSubclassOf<UBDDialogueWidget> DialogueWidgetClass = nullptr;

    UPROPERTY()
    TObjectPtr<UBDDialogueWidget> DialogueWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hint)
    FText HintText = FText::FromString("pick cube");

    int DialoguePage = 0;

    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    EBDCustomerStates CustomerState = EBDCustomerStates::Hungry;

    TObjectPtr<ABDFoodTray> CurrentFood;

    // TObjectPtr<ABDOrderManager> OrderManager;
    FTimerHandle TimerHandle;
    float TimeRemaining = 0.0f;

    FTimerHandle PendingTimerHandle;
    FTimerHandle CookingTimerHandle;
    FTimerHandle HungryAgainTimerHandle;
    FTimerHandle EatTimerHandle;

    void TimerUpdate();

    void SetBlackboardEnumData(FName KeyName, EBDCustomerStates& NewState);

    void PendingTimeOut();
    void CookingTimeOut();
    void EatingTimeOut();
    void HungryAgain();

    bool IsOrderCorrect();

    void Hungry();
    void Ordering();
    void OrderAccepted();
    void OrderReady();
    void Eating();
    void Leaving();
};
