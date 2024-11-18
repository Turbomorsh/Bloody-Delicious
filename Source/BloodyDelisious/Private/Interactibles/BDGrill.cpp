// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDGrill.h"

#include "Components/BoxComponent.h"
#include "Interactibles/BDBurgerPart.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

ABDGrill::ABDGrill()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    SetRootComponent(Mesh);

    CookCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    CookCollision->SetupAttachment(RootComponent);
}

void ABDGrill::Interact(TObjectPtr<UObject> Object)
{
    Super::Interact(Object);
}

void ABDGrill::BeginPlay()
{
    Super::BeginPlay();

    CookCollision->OnComponentBeginOverlap.AddDynamic(this, &ABDGrill::StartCookOverlapped);
    CookCollision->OnComponentEndOverlap.AddDynamic(this, &ABDGrill::ObjectEndOverlap);
}
void ABDGrill::StartCookOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (TObjectPtr<ABDBurgerPart> CastedOtherActor = Cast<ABDBurgerPart>(OtherActor))
    {
        CookingParts.Add(CastedOtherActor);
        FTimerHandle TimerHandle;
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, "Cook", CastedOtherActor);
        GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, CookTime, false);
    }
}

void ABDGrill::ObjectEndOverlap(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (TObjectPtr<ABDBurgerPart> CastedOtherActor = Cast<ABDBurgerPart>(OtherActor))
    {
        CookingParts.Remove(CastedOtherActor);
    }
}

void ABDGrill::StartCook(ABDBurgerPart* OtherActor)
{
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, "Cook", OtherActor);
    GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, CookTime, false);
}

void ABDGrill::Cook(ABDBurgerPart* BurgerPart)
{
    if (CookingParts.Contains(BurgerPart))
    {
        switch (BurgerPart->PartType)
        {
            case EFoodType::Bun:
            {
                if (BunTransformation) BurgerPart->ChangeType(BunTransformation);
                StartCook(BurgerPart);
                break;
            }
            case EFoodType::CockedBun:
            {
                if (CookedBunTransformation) BurgerPart->ChangeType(CookedBunTransformation);
                StartCook(BurgerPart);
                break;
            }
            case EFoodType::Meet:
            {
                if (MeetTransformation) BurgerPart->ChangeType(MeetTransformation);
                StartCook(BurgerPart);
                break;
            }
            case EFoodType::CockedMeet:
            {
                if (CookedMeetTransformation) BurgerPart->ChangeType(CookedMeetTransformation);
                StartCook(BurgerPart);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}