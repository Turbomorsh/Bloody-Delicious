// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/BDPickable.h"
#include "BDBox.generated.h"

enum EFoodType : uint8;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDBox : public ABDPickable
{
    GENERATED_BODY()

public:
    EFoodType PartType;
};
