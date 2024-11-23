// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDDoor.h"

#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/BDInteractionHintWidget.h"

// Sets default values
ABDDoor::ABDDoor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    DoorSocket = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    DoorSocket->SetupAttachment(RootComponent);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door");
    DoorMesh->SetupAttachment(DoorSocket);

    Curve = CreateDefaultSubobject<UCurveFloat>("TimelineCurve");

    BIndTimeline();
}
void ABDDoor::Interact(TObjectPtr<UObject> Object)
{
    if (DoorState != EDoorStates::Locked && !Timeline.IsPlaying())
    {
        if (DoorState == EDoorStates::Closed)
        {
            Timeline.PlayFromStart();
            DoorState = EDoorStates::Opened;
        }
        else if (DoorState == EDoorStates::Opened)
        {
            Timeline.ReverseFromEnd();
            DoorState = EDoorStates::Closed;
        }
    }
}
void ABDDoor::Show()
{
    if (!Hint && HintWidgetClass)
    {
        Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
        Hint->AddToViewport();
        Hint->SetText(FText::FromString("open door"));
    }
}
void ABDDoor::Hide()
{
    if (Hint && HintWidgetClass)
    {
        Hint->RemoveFromParent();
        Hint = nullptr;
    }
}

// Called when the game starts or when spawned
void ABDDoor::BeginPlay()
{
    Super::BeginPlay();

    InitRotator = DoorSocket->GetRelativeRotation();
}

void ABDDoor::BIndTimeline()
{
    Curve->FloatCurve.AddKey(0, 0);
    Curve->FloatCurve.AddKey(1, 1);

    Timeline = FTimeline{};
    FOnTimelineFloat ProgressFunction{};
    ProgressFunction.BindUFunction(this, "TimelineProgress");
    Timeline.AddInterpFloat(Curve, ProgressFunction, "Float1", "FloatTrack");
}

void ABDDoor::TimelineProgress(float Alpha)
{
    int StartRot = InitRotator.Yaw;

    int EndRot = InitRotator.Yaw + 90;

    DoorSocket->SetRelativeRotation(FRotator(DoorSocket->GetRelativeRotation().Pitch, UKismetMathLibrary::Lerp(StartRot, EndRot, Alpha),
        DoorSocket->GetRelativeRotation().Roll));
}

// Called every frame
void ABDDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Timeline.IsPlaying()) Timeline.TickTimeline(DeltaTime);
}

void ABDDoor::Scream()
{
    if (DoorState != EDoorStates::Closed && ScreamSound)
    {
        Timeline.Reverse();
        UGameplayStatics::PlaySoundAtLocation(this, ScreamSound, GetActorLocation());
    }
}
