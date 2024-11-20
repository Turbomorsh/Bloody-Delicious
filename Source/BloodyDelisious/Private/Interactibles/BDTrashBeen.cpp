// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDTrashBeen.h"

#include "Interactibles/BDPlate.h"

ABDTrashBeen::ABDTrashBeen()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    SetRootComponent(Mesh);

    TrashSocket = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    TrashSocket->SetupAttachment(GetRootComponent());
}
void ABDTrashBeen::Interact(TObjectPtr<UObject> Object)
{
    if (TObjectPtr<ABDPlate> CastedPlate = Cast<ABDPlate>(Object))
    {
        ClearPlate(CastedPlate);
    }
}
void ABDTrashBeen::ClearPlate(TObjectPtr<ABDPlate> InPlate)
{
    TArray<AActor*> Burger;
    InPlate->GetAttachedActors(Burger);
    InPlate->Grab(TrashSocket);

    for (int i = 0; i < Burger.Num(); i++)
    {
        Burger[i]->Destroy();
    }

    InPlate->Destroy();
}