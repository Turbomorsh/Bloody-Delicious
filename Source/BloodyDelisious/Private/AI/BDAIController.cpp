// Bloody Delisious. All Rights Reserved.

#include "AI/BDAIController.h"
#include "AI/BDAICharacter.h"
#include "Components/BDAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDAIController, All, All);

ABDAIController::ABDAIController()
{
    BDAIPerceptionComponent = CreateDefaultSubobject<UBDAIPerceptionComponent>("BDPerceptionComponent");
    SetPerceptionComponent(*BDAIPerceptionComponent);
}

void ABDAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogBDAIController, Display, TEXT("%s  OnPossess"), *InPawn->GetName());
    const auto AICharacter = Cast<ABDAICharacter>(InPawn);
    if (AICharacter)
    {
        RunBehaviorTree(AICharacter->BehsaviorTreeAsset);
        AICharacter->OnCustomerStateChanged.AddUObject(this, &ThisClass::SetCustomerFocus);

        UE_LOG(LogBDAIController, Display, TEXT("%s  RunBehavior"), *AICharacter->GetName());
    }
}

void ABDAIController::Tick(float DelataTime)
{
    Super::Tick(DelataTime);

    const auto TargetActor = GetFocusOnActor();
    SetFocus(TargetActor);
}

TObjectPtr<AActor> ABDAIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent() || !bIsFocusSet) return nullptr;

    const auto TargetActor = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
    if (TargetActor != nullptr)
    {
        UE_LOG(LogBDAIController, Display, TEXT("TargetActor %s  "), *TargetActor->GetName());
    }
    return TargetActor;
}

void ABDAIController::SetCustomerFocus(EBDCustomerStates InState)
{
    bIsFocusSet = (InState == EBDCustomerStates::Ordering || InState == EBDCustomerStates::OrderAccepted);
}
