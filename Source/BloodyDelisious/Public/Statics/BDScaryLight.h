// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/BDHorrorInterface.h"
#include "GameFramework/Actor.h"
#include "BDScaryLight.generated.h"

class USpotLightComponent;
class UPointLightComponent;
UCLASS()
class BLOODYDELISIOUS_API ABDScaryLight : public AActor, public IBDHorrorInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDScaryLight();

    virtual void Scream() override;
    virtual void DisableScream() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void LightOffOneTime(float Delay = 0.2f);
    void LightOffOneTime_Implementation(float Delay = 0.2f);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void LightOffDelayed(float Delay = 0.2f, int32 Times = 5);
    void LightOffDelayed_Implementation(float Delay = 0.2f, int32 Times = 5);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Root")
    TObjectPtr<USceneComponent> Scene;

    TArray<USpotLightComponent*> LightComponents;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Timeline")
    TObjectPtr<UCurveFloat> CurveClass;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Light")
    TObjectPtr<USpotLightComponent> SpotLight;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Light")
    TObjectPtr<USpotLightComponent> SpotLight1;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Light")
    TObjectPtr<USpotLightComponent> SpotLight2;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Light")
    TObjectPtr<USpotLightComponent> SpotLight3;

    void SetupLight();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
