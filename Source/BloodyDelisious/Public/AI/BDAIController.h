// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Framework/BDCoreTypes.h"
#include "BDAIController.generated.h"

class UBDAIPerceptionComponent;

UCLASS()
class BLOODYDELISIOUS_API ABDAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABDAIController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UBDAIPerceptionComponent* BDAIPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "TargetActor";

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DelataTime) override;

private:
    bool bIsFocusSet = false;

    TObjectPtr<AActor> GetFocusOnActor() const;

    void SetCustomerFocus(EBDCustomerStates InState);
};
