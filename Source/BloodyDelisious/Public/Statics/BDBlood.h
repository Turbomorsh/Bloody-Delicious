// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "Framework/BDHorrorInterface.h"
#include "BDBlood.generated.h"

/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDBlood : public APaperSpriteActor, public IBDHorrorInterface
{
    GENERATED_BODY()

public:
    virtual void Scream() override;

protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Horror)
    int32 ScreamCost;
};
