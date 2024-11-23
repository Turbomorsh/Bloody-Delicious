// Bloody Delisious. All Rights Reserved.

#include "AI/EQS/BDTargetActorEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AI/BDAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDTargetActorEnvQueryContext, All, All);

void UBDTargetActorEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

    const auto BDAICharacter = Cast<ABDAICharacter>(QueryOwner);
    if (!BDAICharacter) return;

    AActor* FoundActor = nullptr;
    switch (BDAICharacter->GetCustomerState())
    {
        case EBDCustomerStates::Hungry:
            FoundActor = GetActorWithTag(OrderPlaceTag);
            break;
        case EBDCustomerStates::Eating:
            FoundActor = GetActorWithTag(TableTag);
            break;
        case EBDCustomerStates::Leaving:
            FoundActor = GetActorWithTag(OutsidePlaceTag);
            break;
        default:
            break;
    }
    if (FoundActor == nullptr)
    {
        // UE_LOG(LogBDTargetActorEnvQueryContext, Display, TEXT("Actor is nullptr!"));
    }
    else
    {
        UE_LOG(LogBDTargetActorEnvQueryContext, Display, TEXT("Actor %s tag found!"), *FoundActor->GetName());
        UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(FoundActor));
    }
}

AActor* UBDTargetActorEnvQueryContext::GetActorWithTag(FName Tag) const
{
    if (!GetWorld()) return nullptr;

    TArray<AActor*> ActorsWithTag;

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, ActorsWithTag);

    if (ActorsWithTag.Num() > 0)
    {
        return ActorsWithTag[0];
    }

    UE_LOG(LogBDTargetActorEnvQueryContext, Warning, TEXT("No actor with %s tag found!"), *Tag.ToString());
    return nullptr;
}
