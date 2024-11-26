// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BDHorrorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBDHorrorInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class BLOODYDELISIOUS_API IBDHorrorInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual void Scream() = 0;
};
