// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDPickable.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/BDInteractionHintWidget.h"

// Sets default values
ABDPickable::ABDPickable()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(Mesh);
}

void ABDPickable::Interact() {}
void ABDPickable::Show()
{
    if (!Hint)
    {
        Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
        Hint->AddToViewport();
        Hint->SetText(HintText);
    }
}
void ABDPickable::Hide()
{
    if (Hint)
    {
        Hint->RemoveFromParent();
        Hint = nullptr;
    }
}

// Called when the game starts or when spawned
void ABDPickable::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABDPickable::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void ABDPickable::Drop(FRotator CameraRotation, FVector CameraLocation)
{
    const FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
    DetachFromActor(DetachmentRules);

    SetActorEnableCollision(true);
    Mesh->SetSimulatePhysics(true);
    SetActorLocation(CameraLocation + UKismetMathLibrary::GetForwardVector(CameraRotation) * 100);
    Mesh->AddImpulse(UKismetMathLibrary::GetForwardVector(CameraRotation) * 200);
}
void ABDPickable::Grab(USceneComponent* Socket)
{
    Mesh->SetSimulatePhysics(false);
    SetActorLocation(Socket->GetComponentLocation());

    const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
    AttachToComponent(Socket, AttachmentRules);
    SetActorEnableCollision(false);
}
