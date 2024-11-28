// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BDStaticInteractionActor.h"
#include "GameFramework/Actor.h"
#include "BDBoxGiver.generated.h"

class ABDBox;
class UBDInteractionHintWidget;
class ABDPlayerCharacter;
class UBDFoodPartDataAsset;
class ABDBurgerPart;
UCLASS()
class BLOODYDELISIOUS_API ABDBoxGiver : public ABDStaticInteractionActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDBoxGiver();

    virtual void Interact(TObjectPtr<UObject> Object) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TSubclassOf<ABDBox> BoxClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TObjectPtr<UBDFoodPartDataAsset> FoodDataAsset;

    void GiveBox(TObjectPtr<ABDPlayerCharacter> Player);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
