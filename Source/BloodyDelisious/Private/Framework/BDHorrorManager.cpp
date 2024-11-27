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

    OnOrderScoreChanged.AddUObject(this, &ThisClass::OrderScoreChanged);
}

void UBDHorrorManager::OrderScoreChanged(int32 InHorrorScore, int32 InAntiHorrorScore, int32 InFineScore)
{
    ///
    HorrorScore += InHorrorScore;
    HorrorScore += InAntiHorrorScore;
    FineScore += InFineScore;

    UE_LOG(LogTemp, Warning, TEXT("HorrorScore %i, FineScore %i"), HorrorScore, FineScore);
}

void UBDHorrorManager::AddScore(int InScore)
{
    HorrorScore += InScore;
    // UE_LOG(LogTemp, Warning, TEXT("HorrorScore %i, FineScore %i"), HorrorScore, FineScore);

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
    HorrorScore += InScore;

    UE_LOG(LogTemp, Warning, TEXT("SubmissionScore %i"), SubmissionScore);

    if (true)
    {
        StartUpHorrorEvent();
    }
}

void UBDHorrorManager::OnChangedResistansScore(int32 InScore)
{
    FineScore += InScore;
}

void UBDHorrorManager::StartUpHorrorEvent()
{
    TArray<TSubclassOf<AActor>> Keys;
    int32 Random = UKismetMathLibrary::RandomInteger(Keys.Num() - 1);
    HorrorMap.GetKeys(Keys);

<<<<<<< HEAD
    switch (6)
        == == == = if (HorrorScore >= HorrorMap.FindRef(Keys[Random]))
>>>>>>> main
        {
            TArray<AActor*> HorrorActors;
            UGameplayStatics::GetAllActorsOfClass(this, Keys[Random], HorrorActors);

            for (AActor* HorrorActor : HorrorActors)
            {
<<<<<<< HEAD
                // door sound
                TArray<AActor*> HorrorArray;
                UGameplayStatics::GetAllActorsOfClass(this, ABDDoor::StaticClass(), HorrorArray);
                PlayHorrorEvent(Cast<IBDHorrorInterface>(HorrorArray[UKismetMathLibrary::RandomInteger(HorrorArray.Num() - 1)]));
                break;
            }
            case 2:
            {
                // lihgt swich
                TArray<AActor*> HorrorArray;
                UGameplayStatics::GetAllActorsOfClass(this, ABDVendingTap::StaticClass(), HorrorArray);
                PlayHorrorEvent(Cast<IBDHorrorInterface>(HorrorArray[UKismetMathLibrary::RandomInteger(HorrorArray.Num() - 1)]));
                break;
            }
            case 3:
            {
                // burger sound
                TArray<AActor*> HorrorArray;
                UGameplayStatics::GetAllActorsOfClass(this, ABDBurgerPart::StaticClass(), HorrorArray);
                if (Cast<IBDHorrorInterface>(HorrorArray[0])->Scream(HorrorScore))
                    == == == = if (IBDHorrorInterface* CastedActor = Cast<IBDHorrorInterface>(HorrorActor))
>>>>>>> main
                    {
                        CastedActor->Scream();
                    }
<<<<<<< HEAD
                else
                {
                    StartUpHorrorEvent();
                }
                break;
            }
            case 4:
            {
                // cheese
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
            case 5:
            {
                // pig

                break;
            }
            default:
            {
                break;
                == == == =
>>>>>>> main
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
