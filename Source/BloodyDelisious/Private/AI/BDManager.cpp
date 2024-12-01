// Bloody Delisious. All Rights Reserved.

#include "AI/BDManager.h"

// Sets default values
ABDManager::ABDManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABDManager::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABDManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
