// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDPickableGiver.h"

#include "Components/BoxComponent.h"
#include "Interactibles/BDPickable.h"
#include "Player/BDPlayerCharacter.h"

ABDPickableGiver::ABDPickableGiver()
{
    RootScene = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(RootScene);

    Collision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    Collision->SetupAttachment(GetRootComponent());
}

void ABDPickableGiver::Interact(TObjectPtr<UObject> Object)
{
    Super::Interact(Object);

    if (TObjectPtr<USceneComponent> CastedScene = Cast<USceneComponent>(Object))
    {
        if (TObjectPtr<ABDPlayerCharacter> CastedPlayer = Cast<ABDPlayerCharacter>(CastedScene->GetOwner())) GiveActor(CastedPlayer);
    }
}

void ABDPickableGiver::GiveActor(TObjectPtr<ABDPlayerCharacter> Player)
{
    if (PickableClass)
    {
        const FTransform SpawnTransform = FTransform(FRotator(0), GetActorLocation());
        FActorSpawnParameters SpawnParams;
        TObjectPtr<ABDPickable> NewItem = GetWorld()->SpawnActor<ABDPickable>(PickableClass, SpawnTransform, SpawnParams);
        Player->GrabItem(NewItem);
    }
}