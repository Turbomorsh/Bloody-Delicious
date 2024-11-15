// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDInteract.h"
#include "GameFramework/Actor.h"
#include "BDStaticInteractionActor.generated.h"

class UBDInteractionHintWidget;
UCLASS()
class BLOODYDELISIOUS_API ABDStaticInteractionActor : public AActor, public IBDInteract
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDStaticInteractionActor();

    virtual void Interact(TObjectPtr<UObject> Object) override;
    virtual void Show() override;
    virtual void Hide() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Note)
    TSubclassOf<UBDInteractionHintWidget> HintWidgetClass = nullptr;

    UPROPERTY()
    TObjectPtr<UBDInteractionHintWidget> Hint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Note)
    FText HintText = FText::FromString("pick cube");

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
