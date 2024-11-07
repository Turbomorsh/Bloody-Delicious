// Bloody Delisious. All Rights Reserved.

#include "UI/BDMenuHUD.h"
#include "UI/BDMenuWidget.h"

void ABDMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    MenuWidget = CreateWidget<UBDMenuWidget>(GetWorld(), MenuWidgetClass);
    check(MenuWidget);

    MenuWidget->AddToViewport();
}
