// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDPickable.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/BDPlayerCharacter.h"
#include "UI/BDInteractionHintWidget.h"

// Sets default values
ABDPickable::ABDPickable()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(MeshComponent);

    Curve = CreateDefaultSubobject<UCurveFloat>("TimelineCurve");
}

// Called when the game starts or when spawned
void ABDPickable::BeginPlay()
{
    Super::BeginPlay();
    BindTimeLine();
}

// Called every frame
void ABDPickable::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Timeline.IsPlaying()) Timeline.TickTimeline(DeltaTime);
}

void ABDPickable::Interact(TObjectPtr<UObject> Object)
{
    if (TObjectPtr<USceneComponent> CastedScene = Cast<USceneComponent>(Object))
    {
        Grab(CastedScene);
    }
}

void ABDPickable::Show()
{
    if (HintWidgetClass && !Hint)
    {
        Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
        Hint->AddToViewport();
        Hint->SetText(HintText);
    }
}
void ABDPickable::Hide()
{
    if (HintWidgetClass && Hint)
    {
        Hint->RemoveFromParent();
        Hint = nullptr;
    }
}

void ABDPickable::BindTimeLine()
{
    if (Curve)
    {
        Curve->FloatCurve.AddKey(0, 0);
        Curve->FloatCurve.AddKey(0.1, 1);
        Timeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

        Timeline = FTimeline{};
        FOnTimelineFloat ProgressFunction{};
        ProgressFunction.BindUFunction(this, "TimelineProgress");
        Timeline.AddInterpFloat(Curve, ProgressFunction, "Float1", "FloatTrack");
    }
}

void ABDPickable::TimelineProgress(float Alpha)
{
    SetActorLocation(FVector(UKismetMathLibrary::VLerp(InitGrabLocation, GrabLocationSocket->GetComponentLocation(), Alpha)));
}

void ABDPickable::Drop(FRotator CameraRotation, FVector CameraLocation)
{
    const FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
    DetachFromActor(DetachmentRules);

    SetActorEnableCollision(true);
    MeshComponent->SetSimulatePhysics(true);

    FVector End = CameraLocation + UKismetMathLibrary::GetForwardVector(CameraRotation) * 100.f;
    FHitResult OutHit;
    FCollisionQueryParams TraceParams;

    GetWorld()->LineTraceSingleByChannel(OutHit, CameraLocation, End, ECollisionChannel::ECC_Visibility, TraceParams);

    if (OutHit.bBlockingHit)
    {
        SetActorRotation(FRotator(0));
        SetActorLocation(FVector(OutHit.Location.X, OutHit.Location.Y, OutHit.Location.Z + 15));
    }
    else
    {
        SetActorLocation(CameraLocation + UKismetMathLibrary::GetForwardVector(CameraRotation) * 100);
        MeshComponent->AddImpulse(UKismetMathLibrary::GetForwardVector(CameraRotation) * 200);
    }
}

void ABDPickable::Grab(USceneComponent* Socket)
{
    MeshComponent->SetSimulatePhysics(false);
    SetActorRotation(Socket->GetComponentRotation());

    const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
    AttachToComponent(Socket, AttachmentRules);
    SetActorEnableCollision(false);

    GrabLocationSocket = Socket;
    InitGrabLocation = GetActorLocation();

    OnGrabbed.Broadcast();

    Timeline.PlayFromStart();
}
