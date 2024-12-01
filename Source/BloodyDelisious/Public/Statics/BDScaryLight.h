// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/BDHorrorInterface.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "BDScaryLight.generated.h"

class ULocalLightComponent;
class UCurveFloat;

UCLASS()
class BLOODYDELISIOUS_API ABDScaryLight : public AActor, public IBDHorrorInterface
{
    GENERATED_BODY()

public:
    ABDScaryLight();

    virtual void Scream() override;
    virtual void DisableScream() override;

    UFUNCTION(BlueprintCallable)
    void StartLightFade(bool bDecrease);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Light Control")
    UCurveFloat* LightIntensityCurve;  // Curve for change in intensity

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float FadeOutSpeed = 1.0f;  // light attenuation rate

private:
    TMap<ULocalLightComponent*, float> LightSources;

    FTimeline LightTimeline;

    bool bDecreaseIntensity = true;

    UFUNCTION()
    void OnTimelineTick(float Value);

    void CollectLightSources();

public:
    virtual void Tick(float DeltaTime) override;
};
