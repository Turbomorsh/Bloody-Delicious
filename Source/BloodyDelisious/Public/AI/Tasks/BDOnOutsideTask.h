// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BDOnOutsideTask.generated.h"

UCLASS()
class BLOODYDELISIOUS_API UBDOnOutsideTask : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBDOnOutsideTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
