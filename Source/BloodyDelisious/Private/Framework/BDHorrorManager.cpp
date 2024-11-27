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

void UBDHorrorManager::OrderScoreChanged(int32 InHorrorScore, int32 InAntiHorrorScore, int32 InFineScore)
{
    HorrorScore += InHorrorScore;
    HorrorScore += InAntiHorrorScore;
    FineScore += InFineScore;

    UE_LOG(LogBDHorrorManager, Display, TEXT("HorrorScore %i, FineScore %i"), HorrorScore, FineScore);
}

void UBDHorrorManager::StartUpHorrorEvent()
{
    TArray<TSubclassOf<AActor>> Keys;
    int32 Random = UKismetMathLibrary::RandomInteger(Keys.Num() - 1);
    HorrorMap.GetKeys(Keys);

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
    }
}

void UBDHorrorManager::PlayHorrorEvent(IBDHorrorInterface* InterfaceActor)
{
    // if (!InterfaceActor->Scream(HorrorScore))
    //{
    //     StartUpHorrorEvent();
    // }
}
