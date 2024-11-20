// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BDMakeOrderTask.generated.h"

UCLASS()
class BLOODYDELISIOUS_API UBDMakeOrderTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBDMakeOrderTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
