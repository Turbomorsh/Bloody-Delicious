// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDVendingTap.h"

#include "Components/BoxComponent.h"
#include "Interactibles/BDCup.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/ShapeElem.h"

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

    CupSocket = CreateDefaultSubobject<USceneComponent>("CupScene Component");
    CupSocket->SetupAttachment(GetRootComponent());

    Curve = CreateDefaultSubobject<UCurveFloat>("CurveFloat");

    BindTimeLine();
}
void ABDVendingTap::Interact(TObjectPtr<UObject> Object)
{
    Super::Interact(Object);

    if (TObjectPtr<ABDCup> CastedCup = Cast<ABDCup>(Object))
        if (CastedCup->Type == EDrinkType::Empty)
        {
            Timeline.PlayFromStart();
            TakeCup(CastedCup);
        }
}

void ABDVendingTap::BeginPlay()
{
    Super::BeginPlay();

    InitialRotator = TapSocket->GetRelativeRotation();
}

void ABDVendingTap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Timeline.IsPlaying()) Timeline.TickTimeline(DeltaTime);
}

void ABDVendingTap::BindTimeLine()
{
    Curve->FloatCurve.AddKey(0, 0);
    Curve->FloatCurve.AddKey(0.5, 1);

    Timeline = FTimeline{};
    FOnTimelineFloat ProgressFunction{};
    ProgressFunction.BindUFunction(this, "TimeLineUpdate");
    Timeline.AddInterpFloat(Curve, ProgressFunction, "Float1", "FloatTrack");

    Timeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
}

void ABDVendingTap::TimeLineUpdate(float Alpha)
{
    int StartRot = InitialRotator.Pitch;

    int EndRot = InitialRotator.Pitch - 40;

    TapSocket->SetRelativeRotation(FRotator(
        UKismetMathLibrary::Lerp(StartRot, EndRot, Alpha), TapSocket->GetRelativeRotation().Yaw, TapSocket->GetRelativeRotation().Roll));

    GEngine->AddOnScreenDebugMessage(98, 5, FColor::Cyan, "Played");
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
    Timeline.Reverse();
    CupReference = nullptr;
}