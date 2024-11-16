// Bloody Delisious. All Rights Reserved.

#include "Components/BDAIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "AIController.h"

AActor* UBDAIPerceptionComponent::GetClosestTargetActor() const
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestTargetActor = nullptr;

    for (const auto PercieveActor : PercieveActors)
    {
        const auto CurrentDistance = FVector::Distance(PercieveActor->GetActorLocation(), Pawn->GetActorLocation());
        if (CurrentDistance < BestDistance)
        {
            BestDistance = CurrentDistance;
            BestTargetActor = PercieveActor;
        }
    }

    return BestTargetActor;
}
