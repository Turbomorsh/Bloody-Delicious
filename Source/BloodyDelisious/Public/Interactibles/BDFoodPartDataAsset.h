// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BDFoodPartDataAsset.generated.h"

enum EFoodType : uint8;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API UBDFoodPartDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TEnumAsByte<EFoodType> Type;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UMaterial> Material;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UBDFoodPartDataAsset> AlterTransform;
};
