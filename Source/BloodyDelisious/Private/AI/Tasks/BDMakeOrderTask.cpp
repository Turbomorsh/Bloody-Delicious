// Bloody Delisious. All Rights Reserved.

#include "AI/Tasks/BDMakeOrderTask.h"
#include "AIController.h"
#include "AI/BDAICharacter.h"

UBDMakeOrderTask::UBDMakeOrderTask()
{
    NodeName = "Make Order";
}

EBTNodeResult::Type UBDMakeOrderTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto Customer = Cast<ABDAICharacter>(Pawn);
    if (!Customer) return EBTNodeResult::Failed;

    Customer->MakeOrder();
    return EBTNodeResult::Succeeded;
}
