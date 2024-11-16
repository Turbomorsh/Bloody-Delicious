// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "BDAIPerceptionComponent.generated.h"

UCLASS()
class BLOODYDELISIOUS_API UBDAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    AActor* GetClosestTargetActor() const;
};
