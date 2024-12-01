// Bloody Delisious. All Rights Reserved.

#include "AI/BDManager.h"

#include "Camera/CameraComponent.h"

// Sets default values
ABDManager::ABDManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(SceneRoot);

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
    SkeletalMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABDManager::BeginPlay()
{
    Super::BeginPlay();
}

void ABDManager::PlayDialogue_Implementation() {}

void ABDManager::SetOrientation_Implementation() {}

void ABDManager::Kill_Implementation() {}

// Called every frame
void ABDManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
