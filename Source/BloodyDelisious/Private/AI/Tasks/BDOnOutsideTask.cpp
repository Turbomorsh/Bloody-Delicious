// Bloody Delisious. All Rights Reserved.

#include "AI/Tasks/BDOnOutsideTask.h"
#include "AIController.h"
#include "AI/BDAICharacter.h"

UBDOnOutsideTask::UBDOnOutsideTask()
{
    NodeName = "On Outside";
}

EBTNodeResult::Type UBDOnOutsideTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto Customer = Cast<ABDAICharacter>(Pawn);
    if (!Customer) return EBTNodeResult::Failed;

    Customer->OnOutside();
    return EBTNodeResult::Succeeded;
}
