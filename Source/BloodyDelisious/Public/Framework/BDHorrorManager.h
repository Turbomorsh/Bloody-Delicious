// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDHorrorManager.generated.h"

class UBDFoodPartDataAsset;
UCLASS()
class BLOODYDELISIOUS_API ABDHorrorManager : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDHorrorManager();

    UFUNCTION()
    void AddScore(int InScore);

    UFUNCTION()
    void RemoveScore(int InScore);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void PlayHorrorEvent();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    TObjectPtr<UBDFoodPartDataAsset> AlterMeet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    TObjectPtr<UBDFoodPartDataAsset> AlterCheese;

    int HorrorScore = 0;

    int HorrorLimit = 5;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
