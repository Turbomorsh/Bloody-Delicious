// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDManager.generated.h"

UCLASS()
class BLOODYDELISIOUS_API ABDManager : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
