// Bloody Delisious. All Rights Reserved.

#include "AI/Tasks/BDGetTargetActorTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

UBDGetTargetActorTask::UBDGetTargetActorTask()
{
    NodeName = "Next Target Actor";
}

EBTNodeResult::Type UBDGetTargetActorTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !BlackBoard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys) return EBTNodeResult::Failed;

    TArray<AActor*> TargetActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, TargetActors);

    if (TargetActors.Num() != 1) return EBTNodeResult::Failed;

    BlackBoard->SetValueAsObject(TargetActorKey.SelectedKeyName, TargetActors[0]);

    return EBTNodeResult::Succeeded;
}
