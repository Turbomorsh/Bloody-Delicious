// Bloody Delisious. All Rights Reserved.

#include "Framework/BDHorrorManager.h"

#include "AI/BDAICharacter.h"
#include "Interactibles/BDDoor.h"
#include "Interactibles/BDVendingTap.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
UBDHorrorManager::UBDHorrorManager() {}
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

void UBDHorrorManager::PlayHorrorEvent()
{
    int32 RandomInt = UKismetMathLibrary::RandomInteger(5);

    switch (RandomInt)
    {
        case 1:
        {
            TArray<AActor*> AllDoors;
            UGameplayStatics::GetAllActorsOfClass(this, ABDDoor::StaticClass(), AllDoors);
            for (int i = 0; i < AllDoors.Num(); i++)
            {
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
                if (CastedFood->PartType == EFoodType::Meet)
                {
                    CastedFood->ChangeType(AlterMeet);
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
