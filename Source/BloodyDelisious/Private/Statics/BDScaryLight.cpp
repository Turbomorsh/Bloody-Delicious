// Bloody Delisious. All Rights Reserved.

#include "Statics/BDScaryLight.h"

#include "Components/SpotLightComponent.h"

// Sets default values
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

void ABDScaryLight::Scream() {}

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

void ABDScaryLight::LightOffOneTime_Implementation(float Delay) {}

void ABDScaryLight::LightOffDelayed_Implementation(float Delay, int32 Times) {}

// Called every frame
void ABDScaryLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}