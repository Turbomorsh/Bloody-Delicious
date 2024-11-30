// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BDSoundFuncLib.generated.h"

class USoundClass;

UCLASS()
class BLOODYDELISIOUS_API UBDSoundFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static void SetSoundClassVolume(USoundClass* SoundClass, float Volume);

    UFUNCTION(BlueprintCallable)
    static void ToggleSoundClassVolume(USoundClass* SoundClass);
};
