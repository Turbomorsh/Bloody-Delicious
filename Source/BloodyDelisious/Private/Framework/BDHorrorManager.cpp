// Bloody Delisious. All Rights Reserved.

#include "Framework/BDHorrorManager.h"

#include "AI/BDAICharacter.h"
#include "Interactibles/BDDoor.h"
#include "Interactibles/BDVendingTap.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Framework/BDHorrorInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDHorrorManager, All, All);

UBDHorrorManager::UBDHorrorManager()
{
    OnOrderScoreChanged.AddUObject(this, &ThisClass::OrderScoreChanged);
}

void UBDHorrorManager::InitializeHorrorActors()
{
    HorrorActors.Empty();

    for (const auto& Pair : HorrorMap)
    {
        TSubclassOf<AActor> HorrorClass = Pair.Key;
        int32 RequiredScore = Pair.Value;

        if (!HorrorClass)
        {
            UE_LOG(LogBDHorrorManager, Warning, TEXT("HorrorClass is invalid!"));
            continue;
        }

        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), HorrorClass, FoundActors);

        for (AActor* HorrorActor : FoundActors)
        {
            if (HorrorActor)
            {
                HorrorActors.Add(HorrorActor, RequiredScore);
                UE_LOG(LogBDHorrorManager, Display, TEXT("Added HorrorActor %s with RequiredScore %d"),  //
                    *HorrorActor->GetName(), RequiredScore);
            }
        }
    }

    if (HorrorActors.Num() == 0)
    {
        UE_LOG(LogBDHorrorManager, Warning, TEXT("No HorrorActors found for the given classes!"));
    }
}

void UBDHorrorManager::OrderScoreChanged(int32 InHorrorScore, int32 InAntiHorrorScore, int32 InFineScore)
{
    HorrorScore += InHorrorScore;
    HorrorScore += InAntiHorrorScore;
    FineScore += InFineScore;

    FTimerHandle Handle;
    FTimerDelegate Delegate;
    Delegate.BindUFunction(this, "StartUpHorrorEvent", HorrorScore);
    GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, FMath::RandRange(ScreamTimer.Min, ScreamTimer.Max), false);

    UE_LOG(LogBDHorrorManager, Display, TEXT("HorrorScore %i, FineScore %i, InAntiHorrorScore %i"),  //
        HorrorScore, FineScore, InAntiHorrorScore);
}

void UBDHorrorManager::StartUpHorrorEvent(int32 NewHorrorScore)
{
    static TMap<AActor*, bool> ActorActivationStatus;

    for (const auto& Pair : HorrorActors)
    {
        AActor* HorrorActor = Pair.Key;
        int32 RequiredScore = Pair.Value;

        // check status activate
        bool bWasActive = ActorActivationStatus.FindRef(HorrorActor);

        if (NewHorrorScore >= RequiredScore)
        {
            ActivateHorrorActor(HorrorActor, bWasActive, ActorActivationStatus);
        }
        else
        {
            DeactivateHorrorActor(HorrorActor, bWasActive, ActorActivationStatus);
        }
    }
}

void UBDHorrorManager::ActivateHorrorActor(AActor* HorrorActor, bool bWasActive, TMap<AActor*, bool>& ActorActivationStatus)
{
    if (bWasActive) return;

    if (IBDHorrorInterface* CastedActor = Cast<IBDHorrorInterface>(HorrorActor))
    {
        CastedActor->Scream();
        ActorActivationStatus.Add(HorrorActor, true);

        UE_LOG(LogBDHorrorManager, Display, TEXT("HorrorActor %s activated (Scream)."), *HorrorActor->GetName());
    }
    else
    {
        UE_LOG(LogBDHorrorManager, Warning, TEXT("HorrorActor %s does not implement IBDHorrorInterface!"), *HorrorActor->GetName());
    }
}

void UBDHorrorManager::DeactivateHorrorActor(AActor* HorrorActor, bool bWasActive, TMap<AActor*, bool>& ActorActivationStatus)
{
    if (!bWasActive) return;

    if (IBDHorrorInterface* CastedActor = Cast<IBDHorrorInterface>(HorrorActor))
    {
        CastedActor->DisableScream();
        ActorActivationStatus.Add(HorrorActor, false);

        UE_LOG(LogBDHorrorManager, Display, TEXT("HorrorActor %s deactivated (Disable)."), *HorrorActor->GetName());
    }
    else
    {
        UE_LOG(LogBDHorrorManager, Warning, TEXT("HorrorActor %s does not implement IBDHorrorInterface!"), *HorrorActor->GetName());
    }
}
