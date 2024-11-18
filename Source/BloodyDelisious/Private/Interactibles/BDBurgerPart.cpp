// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDBurgerPart.h"

#include "Interactibles/BDFoodPartDataAsset.h"

ABDBurgerPart::ABDBurgerPart()
{
    FoodSocket = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    FoodSocket->SetupAttachment(GetRootComponent());
}

void ABDBurgerPart::BeginPlay()
{
    Super::BeginPlay();
}

void ABDBurgerPart::ChangeType(TObjectPtr<UBDFoodPartDataAsset> Data)
{
    PartType = Data->Type;
    MeshComponent->SetStaticMesh(Data->Mesh);
    MeshComponent->SetMaterial(0, Data->Material);
}