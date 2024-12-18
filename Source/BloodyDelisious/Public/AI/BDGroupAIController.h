// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Framework/BDCoreTypes.h"
#include "BDGroupAIController.generated.h"

class ABDManager;
UCLASS()
class BLOODYDELISIOUS_API ABDGroupAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABDGroupAIController();

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName ToNPCStartTag{"NPC"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Customers")
    TMap<TSubclassOf<APawn>, int32> CustomersTypeByNum;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Customers")
    TMap<TSubclassOf<APawn>, int32> EvilCustomersTypeByNum;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Customers")
    FDelayTime CustomerVisitTimer;

    void InitializeCustomerGroup(
        const TMap<TSubclassOf<APawn>, int32>& GroupData, TArray<AAIController*>& ControllersArray, TArray<APawn*>& PawnsGroup);

protected:
    void ActivateRandomCharacter();

    virtual void BeginPlay() override;

    void OnRoundStart();

    void OnRoundEnd();

private:
    FTimerHandle CustomerVisitHandle;
    bool bIsRoundEnd = false;
    TArray<AAIController*> CustomerControllers;
    TArray<AAIController*> EvilControllers;
    TArray<APawn*> CustomerPawns;
    TArray<APawn*> EvilPawns;

    AActor* GetRandomPlayerStartByTag();
    void SetRandomVisitTimer();
};
