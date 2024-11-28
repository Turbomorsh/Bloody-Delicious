// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDTrashBeen.h"

#include "Interactibles/BDFoodTray.h"
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

    if (TObjectPtr<ABDFoodTray> CastedTray = Cast<ABDFoodTray>(Object))
    {
        ClearTray(CastedTray);
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
void ABDTrashBeen::ClearTray(TObjectPtr<ABDFoodTray> InTray)
{
    TArray<AActor*> Items;
    InTray->GetAttachedActors(Items);
    InTray->Grab(TrashSocket);

    for (AActor* ThisItem : Items)
    {
        if (ABDPlate* CastedPlate = Cast<ABDPlate>(ThisItem))
        {
            ClearPlate(CastedPlate);
        }
        else
        {
            ThisItem->Destroy();
        }
    }

    InTray->Destroy();
}