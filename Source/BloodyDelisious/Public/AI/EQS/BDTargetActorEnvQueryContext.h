// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "BDTargetActorEnvQueryContext.generated.h"

class AActor;

UCLASS()
class BLOODYDELISIOUS_API UBDTargetActorEnvQueryContext : public UEnvQueryContext
{
    GENERATED_BODY()

public:
    virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName TargetActorKeyName = "TargetActor";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName TableTag = "Table";
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName OrderPlaceTag = "OrderPlace";
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName OutsidePlaceTag = "OutsidePlace";

    AActor* GetActorWithTag(FName Tag) const;
};
