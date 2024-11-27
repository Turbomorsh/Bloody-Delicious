// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/BDHorrorInterface.h"
#include "Interactibles/BDStaticInteractionActor.h"
#include "BDGrill.generated.h"

class UBDFoodPartDataAsset;
class ABDBurgerPart;
class UBoxComponent;
/**
 *
 */
UCLASS()
class BLOODYDELISIOUS_API ABDGrill : public ABDStaticInteractionActor, public IBDHorrorInterface
{
    GENERATED_BODY()

public:
    ABDGrill();

    virtual void Interact(TObjectPtr<UObject> Object) override;

    virtual void Scream() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
    TObjectPtr<UBoxComponent> CookCollision;

    TArray<TObjectPtr<ABDBurgerPart>> CookingParts;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    float CookTime = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    TObjectPtr<UBDFoodPartDataAsset> TopBunTransformation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    TObjectPtr<UBDFoodPartDataAsset> TopCookedBunTransformation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    TObjectPtr<UBDFoodPartDataAsset> BottomBunTransformation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    TObjectPtr<UBDFoodPartDataAsset> BottomCookedBunTransformation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    TObjectPtr<UBDFoodPartDataAsset> MeetTransformation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooking)
    TObjectPtr<UBDFoodPartDataAsset> CookedMeetTransformation;

    UPROPERTY()
    TObjectPtr<UAudioComponent> AudioComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sounds)
    TObjectPtr<USoundBase> ScreamSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sounds)
    TObjectPtr<USoundBase> CookSound;

    bool HorrorMode;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Horror)
    int32 HorrorCost = 2;

    virtual void BeginPlay() override;

    UFUNCTION()
    void StartCookOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void ObjectEndOverlap(
        UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void StartCook(ABDBurgerPart* OtherActor);

    UFUNCTION()
    void Cook(ABDBurgerPart* BurgerPart);
};
