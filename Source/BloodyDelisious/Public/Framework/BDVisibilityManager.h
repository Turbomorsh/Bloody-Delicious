// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BDVisibilityManager.generated.h"

UCLASS()
class BLOODYDELISIOUS_API UBDVisibilityManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool IsActorVisible(AActor* Actor);

    UFUNCTION(BlueprintCallable)
    void CameraLog();
};
