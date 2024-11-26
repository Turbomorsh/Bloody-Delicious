// Bloody Delisious. All Rights Reserved.

#include "Framework/BDHorrorManager.h"

#include "AI/BDAICharacter.h"
#include "Interactibles/BDDoor.h"
#include "Interactibles/BDVendingTap.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
    int32 Random = UKismetMathLibrary::RandomInteger(4);

    switch (Random)
    {
        case 1:
        {
            TArray<AActor*> HorrorArray;
            UGameplayStatics::GetAllActorsOfClass(this, ABDDoor::StaticClass(), HorrorArray);
            PlayHorrorEvent(Cast<IBDHorrorInterface>(HorrorArray[UKismetMathLibrary::RandomInteger(HorrorArray.Num() - 1)]));
            break;
        }
        case 2:
        {
            TArray<AActor*> HorrorArray;
            UGameplayStatics::GetAllActorsOfClass(this, ABDVendingTap::StaticClass(), HorrorArray);
            PlayHorrorEvent(Cast<IBDHorrorInterface>(HorrorArray[UKismetMathLibrary::RandomInteger(HorrorArray.Num() - 1)]));
            break;
        }
        case 3:
        {
            TArray<AActor*> HorrorArray;
            UGameplayStatics::GetAllActorsOfClass(this, ABDBurgerPart::StaticClass(), HorrorArray);
            if (Cast<IBDHorrorInterface>(HorrorArray[0])->Scream(HorrorScore))
            {
                for (int32 i = 0; i < HorrorArray.Num() - 1; i++)
                {
                    Cast<IBDHorrorInterface>(HorrorArray[i])->Scream(HorrorScore);
                }
            }
            else
            {
                StartUpHorrorEvent();
            }
            break;
        }
        case 4:
        {
            TArray<AActor*> HorrorArray;
            UGameplayStatics::GetAllActorsOfClass(this, ABDAICharacter::StaticClass(), HorrorArray);
            if (Cast<IBDHorrorInterface>(HorrorArray[0])->Scream(HorrorScore))
            {
                for (int32 i = 0; i < HorrorArray.Num() - 1; i++)
                {
                    Cast<IBDHorrorInterface>(HorrorArray[i])->Scream(HorrorScore);
                }
            }
            else
            {
                StartUpHorrorEvent();
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void UBDHorrorManager::PlayHorrorEvent(IBDHorrorInterface* InterfaceActor)
{
    if (!InterfaceActor->Scream(HorrorScore))
    {
        StartUpHorrorEvent();
    }
}
