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

    SpotLight = CreateDefaultSubobject<USpotLightComponent>("SpotLight1");
    SpotLight->SetupAttachment(GetRootComponent());

    SpotLight1 = CreateDefaultSubobject<USpotLightComponent>("SpotLight2");
    SpotLight1->SetupAttachment(GetRootComponent());

    SpotLight2 = CreateDefaultSubobject<USpotLightComponent>("SpotLight3");
    SpotLight2->SetupAttachment(GetRootComponent());

    SpotLight3 = CreateDefaultSubobject<USpotLightComponent>("SpotLight4");
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

void ABDScaryLight::LightOffOneTime(float Delay)
{
    // for(int32 i = 0; i < LightComponents.Num(); i++)
    //{
    FTimerHandle LightHandle;
    FTimerDelegate LightDelegate;

    LightDelegate.BindUFunction(this, "ChangeLightStateWithCout", SpotLight, 0, Delay);
    GetWorldTimerManager().SetTimer(LightHandle, LightDelegate, Delay, false);
    //}
}

void ABDScaryLight::LightOffDelayed(float Delay, int32 Times)
{
    // for(int32 i = 0; i < LightComponents.Num(); i++)
    //{
    FTimerHandle LightHandle;
    FTimerDelegate LightDelegate;

    LightDelegate.BindUFunction(this, "ChangeLightStateWithCout", SpotLight, Times, Delay);
    GetWorldTimerManager().SetTimer(LightHandle, LightDelegate, Delay, false);
    //}
}

void ABDScaryLight::ChangeLightStateWithCout(USpotLightComponent* InSpotlight, int32 Cout, float Delay)
{
    if (InSpotlight)
    {
        InSpotlight->SetVisibility(!InSpotlight->IsVisible());
    }

    if (Cout > 0)
    {
        Cout--;

        FTimerHandle LightHandle;
        FTimerDelegate LightDelegate;

        LightDelegate.BindUFunction(this, "ChangeLightStateWithCout", Cout, Delay);
        GetWorldTimerManager().SetTimer(LightHandle, LightDelegate, Delay, false);
    }
}

// Called every frame
void ABDScaryLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}