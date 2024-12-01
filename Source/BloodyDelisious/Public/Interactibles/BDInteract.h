// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BDInteract.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBDInteract : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class BLOODYDELISIOUS_API IBDInteract
{
    GENERATED_BODY()

public:
    virtual void Interact(TObjectPtr<UObject> Object = nullptr) = 0;

    virtual void Show(TObjectPtr<UObject> InObject = nullptr) = 0;

    virtual void Hide() = 0;
};
