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

void UBDHorrorManager::InitializeHorrorActors() {}

void UBDHorrorManager::OrderScoreChanged(int32 InHorrorScore, int32 InAntiHorrorScore, int32 InFineScore)
{
    HorrorScore += InHorrorScore;
    HorrorScore += InAntiHorrorScore;
    FineScore += InFineScore;

    if (HorrorScore >= HorrorLimit)
    {
        HorrorLimit += 5;
        FTimerHandle Handle;
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "StartUpHorrorEvent");
        GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, ScreamDelayTime, false);
    }

    UE_LOG(LogBDHorrorManager, Display, TEXT("HorrorScore %i, FineScore %i"), HorrorScore, FineScore);

    StartUpHorrorEvent(HorrorScore);
}

void UBDHorrorManager::StartUpHorrorEvent(int32 NewHorrorScore)
{
    TArray<TSubclassOf<AActor>> Keys;
    int32 Random = UKismetMathLibrary::RandomInteger(Keys.Num() - 1);
    HorrorMap.GetKeys(Keys);

    for (const auto& Pair : HorrorMap)
    {
        TSubclassOf<AActor> HorrorClass = Pair.Key;
        int32 RequiredScore = Pair.Value;

        if (NewHorrorScore >= RequiredScore)
        {
            TArray<AActor*> HorrorActors;
            UGameplayStatics::GetAllActorsOfClass(this, HorrorClass, HorrorActors);
            for (AActor* HorrorActor : HorrorActors)
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

    if (HorrorScore >= HorrorMap.FindRef(Keys[Random]))
    {
        TArray<AActor*> HorrorActors;
        UGameplayStatics::GetAllActorsOfClass(this, Keys[Random], HorrorActors);

        for (AActor* HorrorActor : HorrorActors)
        {
            if (IBDHorrorInterface* CastedActor = Cast<IBDHorrorInterface>(HorrorActor))
            {
                CastedActor->Scream();
            }
        }

        FTimerHandle Handle;
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "DisableHorrorEvent", HorrorActors);
        GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, NormalDelayTime, false);
    }
}

void UBDHorrorManager::PlayHorrorEvent(IBDHorrorInterface* InterfaceActor)
{
    // if (!InterfaceActor->Scream(HorrorScore))
    //{
    //     StartUpHorrorEvent();
    // }
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
