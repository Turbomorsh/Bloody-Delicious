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
        PlayHorrorEvent();
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
        PlayHorrorEvent();
    }
}

void UBDHorrorManager::OnChangedResistansScore(int32 InScore) {}

void UBDHorrorManager::PlayHorrorEvent()
{
    // int32 RandomInt = UKismetMathLibrary::RandomInteger(5);
    int32 RandomInt = 1;

    switch (RandomInt)
    {
        case 1:
        {
            UE_LOG(LogTemp, Warning, TEXT(" case 1"));
            TArray<AActor*> AllDoors;
            UGameplayStatics::GetAllActorsOfClass(this, ABDDoor::StaticClass(), AllDoors);
            for (int i = 0; i < AllDoors.Num(); i++)
            {
                UE_LOG(LogTemp, Warning, TEXT(" ABDDoor !"));
                Cast<ABDDoor>(AllDoors[i])->Scream();
            }
            break;
        }
        case 2:
        {
            TArray<AActor*> AllVendingTaps;
            UGameplayStatics::GetAllActorsOfClass(this, ABDVendingTap::StaticClass(), AllVendingTaps);
            for (int i = 0; i < AllVendingTaps.Num(); i++)
            {
                Cast<ABDVendingTap>(AllVendingTaps[i])->HorrorMode = true;
            }
            break;
        }
        case 3:
        {
            TArray<AActor*> AllPeople;
            UGameplayStatics::GetAllActorsOfClass(this, ABDAICharacter::StaticClass(), AllPeople);
            for (int i = 0; i < AllPeople.Num(); i++)
            {
                Cast<ABDAICharacter>(AllPeople[i])->Scream();
            }
            break;
        }
        case 4:
        {
            TArray<AActor*> AllBurgerParts;
            UGameplayStatics::GetAllActorsOfClass(this, ABDBurgerPart::StaticClass(), AllBurgerParts);
            for (int i = 0; i < AllBurgerParts.Num(); i++)
            {
                ABDBurgerPart* CastedFood = Cast<ABDBurgerPart>(AllBurgerParts[i]);
                if (CastedFood->PartType == EFoodType::Meat)
                {
                    CastedFood->ChangeType(AlterMeat);
                }
            }
            break;
        }
        case 5:
        {
            TArray<AActor*> AllBurgerParts;
            UGameplayStatics::GetAllActorsOfClass(this, ABDBurgerPart::StaticClass(), AllBurgerParts);
            for (int i = 0; i < AllBurgerParts.Num(); i++)
            {
                ABDBurgerPart* CastedFood = Cast<ABDBurgerPart>(AllBurgerParts[i]);
                if (CastedFood->PartType == EFoodType::Cheese)
                {
                    CastedFood->ChangeType(AlterCheese);
                }
            }
            break;
        }
        default:
        {
        }
    }
}
