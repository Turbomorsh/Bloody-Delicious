// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/BDPickable.h"
#include "BDCup.generated.h"

UENUM(BlueprintType)
enum EDrinkType : uint32
{
    Empty,
    Cola,
    Soda,
    Orange
};
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDCup : public ABDPickable
{
    GENERATED_BODY()

public:
    ABDCup();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TEnumAsByte<EDrinkType> Type = EDrinkType::Empty;
};
