// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Framework/BDCoreTypes.h"
#include "Framework/BDHorrorInterface.h"
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

UCLASS() class BLOODYDELISIOUS_API ABDAICharacter : public ACharacter, public IBDInteract, public IBDHorrorInterface
{
    GENERATED_BODY()

public:
    ABDAICharacter();

    virtual void Interact(TObjectPtr<UObject> Object) override;
    virtual void Show() override;
    virtual void Hide() override;

    virtual void Scream() override;
    virtual void DisableScream() override;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ExposeOnSpawn))
    TArray<FText> Dialogue;

    void PigScream();

    void WhisperScream();

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

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
    TObjectPtr<USoundBase> ScreamSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> TraySocket;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
    TArray<TObjectPtr<UBDBurgerTypeDataAsset>> OrderTypes;
    TObjectPtr<UBDBurgerTypeDataAsset> OrderType;

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

    // UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Horror)
    // int32 PigScreamCost = 10;

    // UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Horror)
    // int32 WhisperScreamCost = 5;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Horror")
    TObjectPtr<UStaticMeshComponent> PigHead;

    virtual void BeginPlay() override;

    bool PlayDialogue(TArray<FText> Dialogue, int32 Page);

    void TryGetOrder(TObjectPtr<ABDFoodTray> InOrder);

private:
    EBDCustomerStates CustomerState = EBDCustomerStates::None;
    bool bIsEat = false;

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

    void Hungry();
    void Ordering();
    void OrderAccepted();
    void OrderReady();
    void Eating();
    void Leaving();

    FText GetOrderDescription(const TArray<TEnumAsByte<EFoodType>>& inBurger);
};
