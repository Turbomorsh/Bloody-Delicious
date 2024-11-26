// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDVendingTap.h"

#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "Interactibles/BDCup.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABDVendingTap::ABDVendingTap()
{
    RootScene = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
    SetRootComponent(RootScene);

    TapSocket = CreateDefaultSubobject<USceneComponent>("TapSocketComponent");
    TapSocket->SetupAttachment(GetRootComponent());

    TapCollision = CreateDefaultSubobject<UBoxComponent>("TapBoxCollision");
    TapCollision->SetupAttachment(GetRootComponent());

    TapMesh = CreateDefaultSubobject<UStaticMeshComponent>("TapStaticMesh");
    TapMesh->SetupAttachment(TapSocket);

    CupSocket = CreateDefaultSubobject<USceneComponent>("CupSceneComponent");
    CupSocket->SetupAttachment(GetRootComponent());

    VendingCurve = CreateDefaultSubobject<UCurveFloat>("TimelineCurve");

    BindTimeLine();
}
void ABDVendingTap::Interact(TObjectPtr<UObject> Object)
{
    Super::Interact(Object);

    if (TObjectPtr<ABDCup> CastedCup = Cast<ABDCup>(Object))
        if (CastedCup->Type == EDrinkType::Empty)
        {
            VendingTimeline.PlayFromStart();
            TakeCup(CastedCup);
        }
}
void ABDVendingTap::Scream()
{
    HorrorMode = true;
}

void ABDVendingTap::BeginPlay()
{
    Super::BeginPlay();

    InitialRotator = TapSocket->GetRelativeRotation();
}

void ABDVendingTap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (VendingTimeline.IsPlaying())
    {
        VendingTimeline.TickTimeline(DeltaTime);
    }
}

void ABDVendingTap::BindTimeLine()
{
    VendingCurve->FloatCurve.AddKey(0.f, 0.f);
    VendingCurve->FloatCurve.AddKey(0.5f, 1.f);

    VendingTimeline = FTimeline{};
    FOnTimelineFloat ProgressFunction{};
    ProgressFunction.BindUFunction(this, "TimeLineUpdate");
    VendingTimeline.AddInterpFloat(VendingCurve, ProgressFunction, "Float1", "FloatTrack");
}

void ABDVendingTap::TimeLineUpdate(float Alpha)
{
    int StartRot = InitialRotator.Pitch;

    int EndRot = InitialRotator.Pitch - 40;

    TapSocket->SetRelativeRotation(FRotator(
        UKismetMathLibrary::Lerp(StartRot, EndRot, Alpha), TapSocket->GetRelativeRotation().Yaw, TapSocket->GetRelativeRotation().Roll));
}
void ABDVendingTap::TakeCup(TObjectPtr<ABDCup> InCup)
{
    SetActorEnableCollision(false);
    CupReference = InCup;
    InCup->Grab(CupSocket);
    InCup->Type = DrinkType;
    InCup->OnGrabbed.AddDynamic(this, &ABDVendingTap::ClearCup);
    InCup->SetActorEnableCollision(true);
    if (ScreamSound && PourSound) UGameplayStatics::PlaySoundAtLocation(this, HorrorMode ? ScreamSound : PourSound, GetActorLocation());
}
void ABDVendingTap::ClearCup()
{
    SetActorEnableCollision(true);
    VendingTimeline.Reverse();
    CupReference = nullptr;
}