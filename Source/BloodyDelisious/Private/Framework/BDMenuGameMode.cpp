// Bloody Delisious. All Rights Reserved.

#include "Framework/BDMenuGameMode.h"
#include "Framework/BDUtils.h"

void ABDMenuGameMode::StartPlay()
{
    Super::StartPlay();

    WorldUtils::SetUIInput(GetWorld(), true);
}
