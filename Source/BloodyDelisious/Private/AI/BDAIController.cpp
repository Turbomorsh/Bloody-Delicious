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

    const auto AICharacter = Cast<ABDAICharacter>(InPawn);
    if (AICharacter)
    {
        RunBehaviorTree(AICharacter->BehsaviorTreeAsset);
    }
    // StopBehavior();
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

void ABDAIController::StartBehavior() {}

void ABDAIController::StopBehavior()
{
    FTimerHandle StartBehaviorTimerHandle;
    GetWorldTimerManager().SetTimer(  //
        StartBehaviorTimerHandle,
        [this]()                                                      //
        {                                                             //
            BrainComponent->StopLogic(TEXT("Paused Behavior Tree"));  //
        },
        FMath::RandRange(1.0f, 3.0f), false);
}