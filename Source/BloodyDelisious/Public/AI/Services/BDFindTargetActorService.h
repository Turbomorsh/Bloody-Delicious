// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BDFindTargetActorService.generated.h"

UCLASS()
class BLOODYDELISIOUS_API UBDFindTargetActorService : public UBTService
{
    GENERATED_BODY()

public:
    UBDFindTargetActorService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector TargetActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
