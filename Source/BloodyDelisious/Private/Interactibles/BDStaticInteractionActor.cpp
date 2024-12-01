// Bloody Delisious. All Rights Reserved.

#include "Interactibles/BDStaticInteractionActor.h"

#include "Blueprint/UserWidget.h"
#include "UI/BDInteractionHintWidget.h"

// Sets default values
ABDStaticInteractionActor::ABDStaticInteractionActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}
void ABDStaticInteractionActor::Interact(TObjectPtr<UObject> Object) {}

void ABDStaticInteractionActor::Show(TObjectPtr<UObject> InObject)
{
    if (HintWidgetClass && !Hint)
    {
        if (Cast<USceneComponent>(InObject))
        {
            Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
            Hint->AddToViewport();
            Hint->SetText(HintText);
        }
        else if (AnotherHintTrigger)
            if (InObject->IsA(AnotherHintTrigger))
            {
                Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
                Hint->AddToViewport();
                Hint->SetText(AlterHintText);
            }
    }
}
void ABDStaticInteractionActor::Hide()
{
    if (HintWidgetClass && Hint)
    {
        Hint->RemoveFromParent();
        Hint = nullptr;
    }
}

// Called when the game starts or when spawned
void ABDStaticInteractionActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABDStaticInteractionActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
