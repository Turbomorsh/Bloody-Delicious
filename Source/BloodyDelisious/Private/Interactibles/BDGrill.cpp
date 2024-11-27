// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDGrill.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Interactibles/BDBurgerPart.h"
#include "Kismet/GameplayStatics.h"

ABDGrill::ABDGrill()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    SetRootComponent(Mesh);

    CookCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    CookCollision->SetupAttachment(RootComponent);

    AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
    AudioComponent->SetupAttachment(GetRootComponent());
}

void ABDGrill::Interact(TObjectPtr<UObject> Object)
{
    Super::Interact(Object);
}
void ABDGrill::Scream()
{
    HorrorMode = true;
}
void ABDGrill::DisableScream()
{
    HorrorMode = true;
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

        if (ScreamSound && CookSound)
        {
            if (CookingParts.Num() == 1)
            {
                AudioComponent->SetSound(HorrorMode ? ScreamSound : CookSound);
                AudioComponent->Play();
            }
        }
    }
}

void ABDGrill::ObjectEndOverlap(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (TObjectPtr<ABDBurgerPart> CastedOtherActor = Cast<ABDBurgerPart>(OtherActor))
    {
        CookingParts.Remove(CastedOtherActor);
        if (CookingParts.IsEmpty())
        {
            AudioComponent->Stop();
        }
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
            case EFoodType::TopBun:
            {
                if (TopBunTransformation) BurgerPart->ChangeType(TopBunTransformation);
                StartCook(BurgerPart);
                break;
            }
            case EFoodType::TopCookedBun:
            {
                if (TopCookedBunTransformation) BurgerPart->ChangeType(TopCookedBunTransformation);
                StartCook(BurgerPart);
                break;
            }
            case EFoodType::BottomBun:
            {
                if (BottomBunTransformation) BurgerPart->ChangeType(BottomBunTransformation);
                StartCook(BurgerPart);
                break;
            }
            case EFoodType::BottomCookedBun:
            {
                if (BottomCookedBunTransformation) BurgerPart->ChangeType(BottomCookedBunTransformation);
                StartCook(BurgerPart);
                break;
            }
            case EFoodType::Meat:
            {
                if (MeetTransformation) BurgerPart->ChangeType(MeetTransformation);
                StartCook(BurgerPart);
                break;
            }
            case EFoodType::CookedMeat:
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