// Bloody Delisious. All Rights Reserved.

#include "Player/BDPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDCharacter, All, All);

ABDPlayerCharacter::ABDPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponetn");
    SpringArmComponent->SetupAttachment(GetMesh());
    SpringArmComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("head"));
    SpringArmComponent->TargetArmLength = 0.0f;
    SpringArmComponent->bEnableCameraRotationLag = true;
    SpringArmComponent->CameraRotationLagSpeed = 7.0f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraCompontent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->bUsePawnControlRotation = true;
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

    // set speed
    GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
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
}

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

void ABDPlayerCharacter::PlayerInteract() {}