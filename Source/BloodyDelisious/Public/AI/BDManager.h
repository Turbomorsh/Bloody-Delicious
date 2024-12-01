// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDManager.generated.h"

class UCameraComponent;
UCLASS()
class BLOODYDELISIOUS_API ABDManager : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDManager();

    UFUNCTION(BlueprintNativeEvent)
    void PlayDialogue();
    void PlayDialogue_Implementation();

    UFUNCTION(BlueprintNativeEvent)
    void SetOrientation();
    void SetOrientation_Implementation();

    UFUNCTION(BlueprintNativeEvent)
    void Kill();
    void Kill_Implementation();

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Meshes")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Meshes")
    TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

    int32 SpawnTimes;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
