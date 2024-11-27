// Bloody Delisious. All Rights Reserved.

#include "Statics/BDBlood.h"

void ABDBlood::Scream()
{
    SetActorHiddenInGame(false);
}

void ABDBlood::DisableScream()
{
    SetActorHiddenInGame(true);
}