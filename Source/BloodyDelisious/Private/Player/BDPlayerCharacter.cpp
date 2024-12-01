// Bloody Delisious. All Rights Reserved.

#include "Player/BDPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KismetTraceUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Framework/BDGameMode.h"
#include "Interactibles/BDInteract.h"
#include "Interactibles/BDPickable.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDCharacter, All, All);

ABDPlayerCharacter::ABDPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.3;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponetn");
    SpringArmComponent->SetupAttachment(GetMesh());
    SpringArmComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("head"));
    SpringArmComponent->TargetArmLength = 0.0f;
    SpringArmComponent->bEnableCameraRotationLag = true;
    SpringArmComponent->CameraRotationLagSpeed = 7.0f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraCompontent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->bUsePawnControlRotation = true;

    ItemSocket = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    ItemSocket->SetupAttachment(CameraComponent);

    GetMesh()->bOwnerNoSee = true;
}

void ABDPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (auto* PC = Cast<APlayerController>(GetController()))
    {
        // set Enhanced Input
        if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(CharacterMappingContext, 0);
        }

        // camera settings
        PC->PlayerCameraManager->ViewPitchMax = PitchMax;
        PC->PlayerCameraManager->ViewPitchMin = PitchMin;
        UE_LOG(LogBDCharacter, Display, TEXT("ViewPitchMax: %f | ViewPitchMin: %f"),  //
            PC->PlayerCameraManager->ViewPitchMax, PC->PlayerCameraManager->ViewPitchMin);
    }

    Cast<ABDGameMode>(UGameplayStatics::GetGameMode(this))->OnRoundStart.AddUFunction(this, "StartFade");
    Cast<ABDGameMode>(UGameplayStatics::GetGameMode(this))->OnAllCustomerExited.AddUFunction(this, "EndFade");

    // set speed
    GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}

void ABDPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    InteractionRaycast();
}

void ABDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    auto* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    check(Input);
    if (MoveAction)
    {
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
    }
    else
    {
        UE_LOG(LogBDCharacter, Error, TEXT("MoveAction isn't set!"));
    }

    if (LookAction)
    {
        Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
    }
    else
    {
        UE_LOG(LogBDCharacter, Error, TEXT("LookAction isn't set!"));
    }

    if (InteractAction)
    {
        Input->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::PlayerInteract);
    }
    else
    {
        UE_LOG(LogBDCharacter, Error, TEXT("InteractAction isn't set!"));
    }

    if (DropAction)
    {
        Input->BindAction(DropAction, ETriggerEvent::Started, this, &ThisClass::DropItem);
    }
    else
    {
        UE_LOG(LogBDCharacter, Error, TEXT("DropAction isn't set!"));
    }
}

void ABDPlayerCharacter::StartFade_Implementation() {}

void ABDPlayerCharacter::EndFade_Implementation() {}

void ABDPlayerCharacter::Move(const FInputActionValue& InputActionValue)
{
    AddMovementInput(GetActorForwardVector(), InputActionValue.Get<FVector2D>().Y);
    AddMovementInput(GetActorRightVector(), InputActionValue.Get<FVector2D>().X);
}

void ABDPlayerCharacter::Look(const FInputActionValue& InputActionValue)
{
    AddControllerYawInput(InputActionValue.Get<FVector2D>().X);
    AddControllerPitchInput(-InputActionValue.Get<FVector2D>().Y);
}

void ABDPlayerCharacter::PlayerInteract()
{
    if (InteractibleObj)
    {
        if (!HandledObj)
        {
            Cast<IBDInteract>(InteractibleObj)->Interact(ItemSocket);

            if (InteractibleObj->GetClass()->IsChildOf(ABDPickable::StaticClass()) && !HandledObj)
            {
                HandledObj = Cast<ABDPickable>(InteractibleObj);
                HandledObj->OnGrabbed.AddDynamic(this, &ABDPlayerCharacter::ClearItemRef);
            }
        }
        else
            Cast<IBDInteract>(InteractibleObj)->Interact(HandledObj);
    }
}

void ABDPlayerCharacter::GrabItem(TObjectPtr<ABDPickable> Item)
{
    if (!HandledObj)
    {
        Item->Grab(ItemSocket);
        HandledObj = Item;
        HandledObj->OnGrabbed.AddDynamic(this, &ABDPlayerCharacter::ClearItemRef);
    }
}

void ABDPlayerCharacter::ClearItemRef()
{
    if (HandledObj)
    {
        HandledObj->OnGrabbed.RemoveDynamic(this, &ABDPlayerCharacter::ClearItemRef);
        HandledObj = nullptr;
    }
}

void ABDPlayerCharacter::DropItem(const FInputActionValue& InputActionValue)
{
    if (HandledObj)
    {
        HandledObj->Drop(CameraComponent->GetComponentRotation(), CameraComponent->GetComponentLocation());
        ClearItemRef();
    }
}

void ABDPlayerCharacter::InteractionRaycast()
{
    FVector Start = CameraComponent->GetComponentLocation();
    FVector End = Start + UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 500.f;
    FHitResult OutHit;
    FCollisionQueryParams TraceParams;

    GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, TraceParams);

    if (InteractibleObj && InteractibleObj != OutHit.GetActor())
    {
        Cast<IBDInteract>(InteractibleObj)->Hide();
        InteractibleObj = nullptr;
    }

    if (UKismetSystemLibrary::DoesImplementInterface(OutHit.GetActor(), UBDInteract::StaticClass()))
    {
        InteractibleObj = OutHit.GetActor();
        Cast<IBDInteract>(InteractibleObj)->Show();
    }
}

void ABDPlayerCharacter::SetHaveCassete(bool InBool)
{
    HasCassete = InBool;

    UE_LOG(LogBDCharacter, Display, TEXT("HaveCassete: %s"), *LexToString(HasCassete));
}