// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDPickable.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/BDPlayerCharacter.h"
#include "UI/BDInteractionHintWidget.h"
#include "Framework/BDGameMode.h"

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

    Cast<ABDGameMode>(GetWorld()->GetAuthGameMode())->OnRoundStart.AddUFunction(this, "Destruct");

    BindTimeLine();
}

void ABDPickable::Destruct()
{
    OnGrabbed.Broadcast();
    Destroy();
}

// Called every frame
void ABDPickable::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PickableTimeline.IsPlaying()) PickableTimeline.TickTimeline(DeltaTime);
}

void ABDPickable::Interact(TObjectPtr<UObject> Object)
{
    if (TObjectPtr<USceneComponent> CastedScene = Cast<USceneComponent>(Object))
    {
        Grab(CastedScene);
    }
}

void ABDPickable::Show(TObjectPtr<UObject> InObject)
{
    if (HintWidgetClass && !Hint)
    {
        if (Cast<USceneComponent>(InObject))
        {
            Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
            Hint->AddToViewport();
            Hint->SetText(HintText);
        }
        else if (AnotherHintTrigger)
            if (InObject->IsA(AnotherHintTrigger))
            {
                Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
                Hint->AddToViewport();
                Hint->SetText(AlterHintText);
            }
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
        Curve->FloatCurve.AddKey(0.0f, 0.0f);
        Curve->FloatCurve.AddKey(0.1f, 1.0f);
        PickableTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

        PickableTimeline = FTimeline{};
        FOnTimelineFloat ProgressFunction{};
        ProgressFunction.BindUFunction(this, "TimelineProgress");
        PickableTimeline.AddInterpFloat(Curve, ProgressFunction, "Float1", "FloatTrack");
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

    PickableTimeline.PlayFromStart();
}
