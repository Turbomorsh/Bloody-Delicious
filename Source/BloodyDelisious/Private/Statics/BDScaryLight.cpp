// Bloody Delisious. All Rights Reserved.

#include "Statics/BDScaryLight.h"

#include "Components/SpotLightComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDScaryLight, All, All);

ABDScaryLight::ABDScaryLight()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Scene = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(Scene);

    SpotLight = CreateDefaultSubobject<USpotLightComponent>("SpotLight");
    SpotLight->SetupAttachment(GetRootComponent());

    SpotLight1 = CreateDefaultSubobject<USpotLightComponent>("SpotLight1");
    SpotLight1->SetupAttachment(GetRootComponent());

    SpotLight2 = CreateDefaultSubobject<USpotLightComponent>("SpotLight2");
    SpotLight2->SetupAttachment(GetRootComponent());

    SpotLight3 = CreateDefaultSubobject<USpotLightComponent>("SpotLight3");
    SpotLight3->SetupAttachment(GetRootComponent());
}

void ABDScaryLight::Scream()
{
    float Dur = FMath::RandRange(1.0f, 3.0f);
    LightOffDelayed(Dur);
    UE_LOG(LogBDScaryLight, Display, TEXT("Scream %f"), Dur);
}

void ABDScaryLight::DisableScream() {}

// Called when the game starts or when spawned
void ABDScaryLight::BeginPlay()
{
    Super::BeginPlay();

    SetupLight();
}

void ABDScaryLight::SetupLight()
{
    LightComponents.Add(SpotLight);
    LightComponents.Add(SpotLight1);
    LightComponents.Add(SpotLight2);
    LightComponents.Add(SpotLight3);
}

void ABDScaryLight::LightOffOneTime_Implementation(float Delay)
{
    // UE_LOG(LogTemp, Log, TEXT("Light turned off after delay: %f"), Delay);
}

void ABDScaryLight::LightOffDelayed_Implementation(float Delay, int32 Times) {}

// Called every frame
void ABDScaryLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}