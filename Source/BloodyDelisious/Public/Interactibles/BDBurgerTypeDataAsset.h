// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDFoodTray.h"
#include "Engine/DataAsset.h"
#include "BDBurgerTypeDataAsset.generated.h"

class ABDBurgerPart;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API UBDBurgerTypeDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    FOrderStruct Order;

    UPROPERTY(EditDefaultsOnly)
    FText OrderName;
};
