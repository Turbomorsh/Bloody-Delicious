// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDBurgerPart.h"

#include "Interactibles/BDFoodPartDataAsset.h"
#include "Framework/BDGameMode.h"

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
    if (Data->AlterTransform)
    {
        AlterTransformData = Data->AlterTransform;
    }
}
void ABDBurgerPart::Scream()
{
    AlterTransform();
}
void ABDBurgerPart::DisableScream() {}

void ABDBurgerPart::AlterTransform()
{
    if (AlterTransformData)
    {
        ChangeType(AlterTransformData);
    }
}