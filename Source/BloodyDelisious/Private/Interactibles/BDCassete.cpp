// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDCassete.h"

#include "Player/BDPlayerCharacter.h"

ABDCassete::ABDCassete()
{
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    SetRootComponent(StaticMesh);
}
void ABDCassete::Interact(TObjectPtr<UObject> Object)
{
    Super::Interact(Object);

    if (USceneComponent* CastedScene = Cast<USceneComponent>(Object)) Grab(CastedScene);
}
void ABDCassete::Grab(USceneComponent* Socket)
{
    Cast<ABDPlayerCharacter>(Socket->GetOwner())->SetHaveCassete(true);
    Destroy();
}