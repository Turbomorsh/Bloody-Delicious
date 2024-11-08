// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BDPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class BLOODYDELISIOUS_API ABDPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABDPlayerCharacter();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> CharacterMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> AttackAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY(EditDefaultsOnly, Category = "BDPlayer | Camera settings", meta = (ClampMin = "0", ClampMax = "90"))
    float PitchMax = 70.0f;

    UPROPERTY(EditDefaultsOnly, Category = "BDPlayer | Camera settings", meta = (ClampMin = "-90", ClampMax = "0"))
    float PitchMin = -70.0f;

    UPROPERTY(EditDefaultsOnly, Category = "BDPlayer | Walk speed", meta = (ClampMin = "0", ClampMax = "600"))
    float MaxSpeed = 212.0f;

    UFUNCTION()
    void Move(const FInputActionValue& InputActionValue);

    UFUNCTION()
    void Look(const FInputActionValue& InputActionValue);

    UFUNCTION()
    void PlayerInteract();

    virtual void BeginPlay() override;
};
