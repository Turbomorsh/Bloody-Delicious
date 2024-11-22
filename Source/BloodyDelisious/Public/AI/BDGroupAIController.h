// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Framework/BDCoreTypes.h"
#include "BDGroupAIController.generated.h"

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

    void InitializeGroups(const TMap<TSubclassOf<APawn>, int32>& GroupData);

protected:
    void ActivateRandomCharacter();

    virtual void BeginPlay() override;

private:
    AActor* GetRandomPlayerStartByTag();

    void OnGameStateChanged(EBDGameState State);

    TMap<TSubclassOf<APawn>, TArray<AAIController*>> CustomerGroups;
};
