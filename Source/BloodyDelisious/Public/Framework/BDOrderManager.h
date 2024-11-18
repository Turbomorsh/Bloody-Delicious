// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Framework/BDCoreTypes.h"
#include "BDOrderManager.generated.h"

class ABDAICharacter;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class BLOODYDELISIOUS_API ABDOrderManager : public AActor
{
    GENERATED_BODY()

public:
    ABDOrderManager();

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<USceneComponent> SceneComponent;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UWidgetComponent> OrderingAriaWidget;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UWidgetComponent> AcceptanceAriaWidget;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UWidgetComponent> CookingAriaWidget;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UWidgetComponent> DeliveryAriaWidget;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> OrderingAriaCollision;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> AcceptanceAriaCollision;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> CookingAriaCollision;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> DeliveryAriaCollision;

    UPROPERTY(EditDefaultsOnly, Category = "Order manager settings", meta = (ClampMin = "0", ClampMax = "120"))
    float TimeToPendingOrder = 5.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "Order manager settings", meta = (ClampMin = "0", ClampMax = "120"))
    float TimeToCooking = 15.0f;  // in sec

    UPROPERTY(EditDefaultsOnly, Category = "Order manager settings")
    TSubclassOf<ACharacter> PlayerClass;

    void SetOrderState(EBDOrderStates NewState) { OrderState = NewState; };

protected:
    virtual void BeginPlay() override;

private:
    EBDOrderStates OrderState = EBDOrderStates::NextInLine;

    // TObjectPtr<ABDAICharacter> CurrentCustomer;

    FTimerHandle PendingTimerHandle;
    FTimerHandle CookingTimerHandle;

    void MakeOrder();
    void ReceiveOrder();
    void CookOrder();
    void DeliverOrder();

    void PendingTimeOut();
    void CookingTimeOut();

    bool IsOrderCorrect();
    bool IsPlayerCharacter(AActor* OtherActor);

    UFUNCTION()
    void OrderingAriaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void AcceptanceAriaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void CookingAriaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void DeliveryAriaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
