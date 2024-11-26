// Bloody Delisious. All Rights Reserved.

#include "Framework/BDHorrorManager.h"

#include "AI/BDAICharacter.h"
#include "Interactibles/BDDoor.h"
#include "Interactibles/BDVendingTap.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Framework/BDHorrorInterface.h"

// Sets default values
UBDHorrorManager::UBDHorrorManager()
{
    OnSubmissionScoreChanged.AddUObject(this, &ThisClass::OnChangedSubmissionScore);
    OnResistansScoreChanged.AddUObject(this, &ThisClass::OnChangedResistansScore);
}

void UBDHorrorManager::AddScore(int InScore)
{
    HorrorScore += InScore;

    if (HorrorScore >= HorrorLimit)
    {
        HorrorLimit += 5;
        FTimerHandle Handle;
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "StartUpHorrorEvent");
        GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, DelayTime, false);
    }
}
void UBDHorrorManager::RemoveScore(int InScore)
{
    HorrorScore -= InScore;

    if (HorrorScore < HorrorLimit) HorrorLimit -= 5;
}

void UBDHorrorManager::OnChangedSubmissionScore(int32 InScore)
{
    SubmissionScore += InScore;

    UE_LOG(LogTemp, Warning, TEXT("SubmissionScore %i"), SubmissionScore);

    if (true)
    {
        StartUpHorrorEvent();
    }
}

void UBDHorrorManager::OnChangedResistansScore(int32 InScore) {}

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
