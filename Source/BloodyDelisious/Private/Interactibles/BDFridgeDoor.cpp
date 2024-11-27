// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDFridgeDoor.h"

#include "Kismet/KismetMathLibrary.h"

ABDFridgeDoor::ABDFridgeDoor()
{
    DoorSocket = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(DoorSocket);

    DoorMesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    DoorMesh->SetupAttachment(GetRootComponent());

    Curve = CreateDefaultSubobject<UCurveFloat>("TimelineCurve");
}

void ABDFridgeDoor::BeginPlay()
{
    Super::BeginPlay();

    BIndTimeline();

    InitRotator = DoorSocket->GetRelativeRotation();
}

void ABDFridgeDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Timeline.IsPlaying())
    {
        Timeline.TickTimeline(DeltaTime);
    }
}

void ABDFridgeDoor::Interact(TObjectPtr<UObject> Object)
{
    Super::Interact(Object);

    if (!IsOpen)
    {
        Timeline.PlayFromStart();
        IsOpen = true;
    }
    else
    {
        Timeline.ReverseFromEnd();
        IsOpen = false;
    }
}

void ABDFridgeDoor::BIndTimeline()
{
    if (!Curve)
    {
        UE_LOG(LogTemp, Error, TEXT("Curve is not assigned!"));
        return;
    }

    Curve->FloatCurve.AddKey(0.0f, 0.0f);
    Curve->FloatCurve.AddKey(1.0f, 1.0f);

    Timeline = FTimeline{};
    FOnTimelineFloat ProgressFunction{};
    ProgressFunction.BindUFunction(this, "TimelineProgress");
    Timeline.AddInterpFloat(Curve, ProgressFunction, "Float1", "FloatTrack");
}

void ABDFridgeDoor::TimelineProgress(float Alpha)
{
    double StartRot = InitRotator.Yaw;

    double EndRot = InitRotator.Yaw + 90.0;

    DoorSocket->SetRelativeRotation(FRotator(DoorSocket->GetRelativeRotation().Pitch, UKismetMathLibrary::Lerp(StartRot, EndRot, Alpha),
        DoorSocket->GetRelativeRotation().Roll));
}