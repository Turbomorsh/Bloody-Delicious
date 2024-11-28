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
                UE_LOG(LogBDHorrorManager, Display, TEXT("Added HorrorActor %s with RequiredScore %d"), *HorrorActor->GetName(),
                    RequiredScore);
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

    UE_LOG(LogBDHorrorManager, Display, TEXT("HorrorScore %i, FineScore %i"), HorrorScore, FineScore);

    // StartUpHorrorEvent(HorrorScore);
}

void UBDHorrorManager::StartUpHorrorEvent(int32 NewHorrorScore)
{
    for (const auto& Pair : HorrorActors)
    {
        AActor* HorrorActor = Pair.Key;
        int32 RequiredScore = Pair.Value;

        if (NewHorrorScore >= RequiredScore)
        {
            if (IBDHorrorInterface* CastedActor = Cast<IBDHorrorInterface>(HorrorActor))
            {
                CastedActor->Scream();
                UE_LOG(LogBDHorrorManager, Display, TEXT("HorrorActor of class %s screamed!"), *HorrorActor->GetName());
            }
            else
            {
                UE_LOG(LogBDHorrorManager, Warning, TEXT("HorrorActor does not implement IBDHorrorInterface!"));
            }
        }
    }
}

void UBDHorrorManager::DisableHorrorEvent(TArray<AActor*> HorroredActors)
{
    for (AActor* HorrorActor : HorroredActors)
    {
        if (IBDHorrorInterface* CastedActor = Cast<IBDHorrorInterface>(HorrorActor))
        {
            CastedActor->DisableScream();
        }
    }
}
