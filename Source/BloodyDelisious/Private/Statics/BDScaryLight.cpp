// Bloody Delisious. All Rights Reserved.

#include "Statics/BDScaryLight.h"

#include "Curves/CurveFloat.h"
#include "Components/LocalLightComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDScaryLight, All, All);

ABDScaryLight::ABDScaryLight()
{
    PrimaryActorTick.bCanEverTick = true;

    LightTimeline = FTimeline{};
}

void ABDScaryLight::BeginPlay()
{
    Super::BeginPlay();

    CollectLightSources();

    if (IsValid(LightIntensityCurve))
    {
        UE_LOG(LogBDScaryLight, Log, TEXT("LightIntensityCurve is valid."));
    }
    else
    {
        UE_LOG(LogBDScaryLight, Warning, TEXT("LightIntensityCurve is invalid!"));
    }

    if (LightIntensityCurve)
    {
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, "OnTimelineTick");
        LightTimeline.AddInterpFloat(LightIntensityCurve, TimelineCallback);
    }
}

/*-----------------------------------------------*/
void ABDScaryLight::Scream()
{
    StartLightFade(true);
    UE_LOG(LogBDScaryLight, Display, TEXT("Scream %s"), *LexToString(true));
}

void ABDScaryLight::DisableScream() {}
/*-----------------------------------------------*/

void ABDScaryLight::CollectLightSources()
{
    if (!GetWorld())
    {
        UE_LOG(LogBDScaryLight, Error, TEXT("No valid World found!"));
        return;
    }

    LightSources.Empty();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        TArray<ULocalLightComponent*> Components;
        Actor->GetComponents(Components);

        for (ULocalLightComponent* Light : Components)
        {
            if (Light)
            {
                LightSources.Add(Light, Light->Intensity);  // Setup map

                // Log
                const FString LightName = Light->GetOwner() ? Light->GetOwner()->GetName() : TEXT("Unknown");
                UE_LOG(LogBDScaryLight, Log, TEXT("Found Light: %s, Intensity: %f"), *LightName, Light->Intensity);
            }
        }
    }
    UE_LOG(LogBDScaryLight, Log, TEXT("Collected %d light sources."), LightSources.Num());
}

void ABDScaryLight::StartLightFade(bool bDecrease)
{
    bDecreaseIntensity = bDecrease;

    if (LightTimeline.IsPlaying())
    {
        LightTimeline.Stop();
    }

    LightTimeline.PlayFromStart();
}

void ABDScaryLight::OnTimelineTick(float Value)
{
    UE_LOG(LogBDScaryLight, Log, TEXT("Timeline Alpha: %f"), Value);
    for (auto& Pair : LightSources)
    {
        if (Pair.Key)
        {
            const float TargetIntensity = bDecreaseIntensity ? 0.0f : Pair.Value;
            const float AdjustedAlpha = FMath::Clamp(Value * FadeOutSpeed, 0.0f, 1.0f);
            const float NewIntensity = FMath::Lerp(Pair.Value, TargetIntensity, AdjustedAlpha);
            Pair.Key->SetIntensity(NewIntensity);

            // Log
            const FString LightName = Pair.Key->GetOwner() ? Pair.Key->GetOwner()->GetName() : TEXT("Unknown");
            UE_LOG(LogBDScaryLight, Log, TEXT("Light: %s, Current Intensity: %f, Target Intensity: %f, Alpha: %f"), *LightName,
                NewIntensity, TargetIntensity, Value);
        }
    }
}

void ABDScaryLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (LightTimeline.IsPlaying())
    {
        LightTimeline.TickTimeline(DeltaTime);
    }
}
