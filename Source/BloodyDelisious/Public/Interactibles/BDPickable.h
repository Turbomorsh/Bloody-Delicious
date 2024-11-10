// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDInteract.h"
#include "GameFramework/Actor.h"
#include "BDPickable.generated.h"

class UBDInteractionHintWidget;
UCLASS()
class BLOODYDELISIOUS_API ABDPickable : public AActor, public IBDInteract
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDPickable();

    void Interact() override;

    void Show() override;

    void Hide() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Note)
    TSubclassOf<UBDInteractionHintWidget> HintWidgetClass;

    UPROPERTY()
    TObjectPtr<UBDInteractionHintWidget> Hint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Note)
    FText HintText = FText::FromString("pick cube");

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void Drop(FRotator CameraRotation, FVector CameraLocation);

    void Grab(USceneComponent* Socket);
};
