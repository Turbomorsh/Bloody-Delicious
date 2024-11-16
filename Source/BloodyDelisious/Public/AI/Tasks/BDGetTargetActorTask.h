// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BDGetTargetActorTask.generated.h"

UCLASS()
class BLOODYDELISIOUS_API UBDGetTargetActorTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBDGetTargetActorTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector TargetActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName TargetTag = "None";
};
