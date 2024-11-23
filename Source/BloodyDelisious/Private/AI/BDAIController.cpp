// Bloody Delisious. All Rights Reserved.

#include "AI/BDAIController.h"
#include "AI/BDAICharacter.h"
#include "Components/BDAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

ABDAIController::ABDAIController()
{
    BDAIPerceptionComponent = CreateDefaultSubobject<UBDAIPerceptionComponent>("BDPerceptionComponent");
    SetPerceptionComponent(*BDAIPerceptionComponent);
}

void ABDAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogTemp, Warning, TEXT("%s  OnPossess"), *InPawn->GetName());
    const auto AICharacter = Cast<ABDAICharacter>(InPawn);
    if (AICharacter)
    {
        RunBehaviorTree(AICharacter->BehsaviorTreeAsset);
        // AICharacter->OnCustomerOutside.AddUObject(this, &ThisClass::StopBehavior);
        UE_LOG(LogTemp, Warning, TEXT("%s  RunBehavior"), *AICharacter->GetName());
    }
    // BrainComponent->StopLogic(TEXT("Paused Behavior Tree"));
}

void ABDAIController::Tick(float DelataTime)
{
    Super::Tick(DelataTime);

    const auto TargetActor = GetFocusOnActor();
    SetFocus(TargetActor);
}

TObjectPtr<AActor> ABDAIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent()) return nullptr;

    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
