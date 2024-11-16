// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Framework/BDCoreTypes.h"
#include "BDAICharacter.generated.h"

class UBehaviorTree;
class ABDOrderManager;

UCLASS() class BLOODYDELISIOUS_API ABDAICharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABDAICharacter();

    void SetCustomerState(EBDCustomerStates NewState);

    UFUNCTION(BlueprintCallable)
    EBDCustomerStates GetCustomerState() const { return CustomerState; };

    void SetOrderManagerPtr(TObjectPtr<ABDOrderManager> InOrderManager) { OrderManager = InOrderManager; };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TObjectPtr<UBehaviorTree> BehsaviorTreeAsset;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (ClampMin = "0", ClampMax = "600"))
    float MaxSpeed = 212.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName CustomerStatusKeyName = "CustomerStatus";

    UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (ClampMin = "0", ClampMax = "600"))
    float TimeHungryAgain = 10.0f;  // in sec

    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    EBDCustomerStates CustomerState = EBDCustomerStates::Hungry;

    TObjectPtr<ABDOrderManager> OrderManager;

    FTimerHandle HungryAgainTimerHandle;

    void SetBlackboardEnumData(FName KeyName, EBDCustomerStates& NewState);

    void HungryAgain();

    void Hungry();
    void Ordering();
    void OrderAccepted();
    void OrderReady();
    void Eating();
    void Leaving();
};
