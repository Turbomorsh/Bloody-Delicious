// Bloody Delisious. All Rights Reserved.

#include "AI/Services/BDFindTargetActorService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BDAIPerceptionComponent.h"

UBDFindTargetActorService::UBDFindTargetActorService()
{
    NodeName = "Find Target";
}

void UBDFindTargetActorService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if (BlackBoard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = Controller->FindComponentByClass<UBDAIPerceptionComponent>();
        if (PerceptionComponent)
        {
            BlackBoard->SetValueAsObject(TargetActorKey.SelectedKeyName, PerceptionComponent->GetClosestTargetActor());
        }
    }
}
