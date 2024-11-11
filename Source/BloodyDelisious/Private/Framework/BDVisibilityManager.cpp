// Bloody Delisious. All Rights Reserved.

#include "Framework/BDVisibilityManager.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDVisibilityManager, All, All);

bool UBDVisibilityManager::IsActorVisible(AActor* Actor)
{
    if (!IsValid(Actor)) return false;

    auto* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return false;

    // get screen size
    int32 SizeX, SizeY;
    PC->GetViewportSize(SizeX, SizeY);

    // initialize array
    TArray<FVector> VisiblePoints;
    VisiblePoints.Reserve(8);  // reserve memory

    FBox ActorBounds = Actor->GetComponentsBoundingBox();
    FVector Corners[8];
    ActorBounds.GetVertices(Corners);

    // set trace parameters
    FHitResult HitResult;
    FCollisionQueryParams TraceParams(FName(TEXT("VisibilityTrace")), true, Actor);

    auto IsPointInFrustum = [&](const FVector& ScreenLocation) -> bool
    {
        return ScreenLocation.X >= 0 && ScreenLocation.X <= SizeX &&  //
               ScreenLocation.Y >= 0 && ScreenLocation.Y <= SizeY;
    };

    for (const FVector& Corner : Corners)
    {
        FVector ScreenLocation;
        if (!PC->ProjectWorldLocationToScreenWithDistance(Corner, ScreenLocation)) continue;
        if (!IsPointInFrustum(ScreenLocation)) continue;

        bool bHitObstacle = GetWorld()->LineTraceSingleByChannel(  //
            HitResult,                                             //
            PC->PlayerCameraManager->GetCameraLocation(),          //
            Corner,                                                //
            ECC_Visibility,                                        // need block visibility channel for mesh testing actor
            TraceParams);

        DrawDebugPoint(GetWorld(), Corner, 2.0f,                                          //
            bHitObstacle && HitResult.GetActor() != Actor ? FColor::Red : FColor::Green,  //
            false, -1.0f, 1);

        if (!bHitObstacle || HitResult.GetActor() == Actor)
        {
            VisiblePoints.Add(Corner);
        }
    }

    UE_LOG(LogBDVisibilityManager, Display, TEXT("Actor %s is %s"), *Actor->GetName(),
        VisiblePoints.IsEmpty() ? TEXT("HIDDEN") : TEXT("VISIBLE"));

    return !VisiblePoints.IsEmpty();
}

void UBDVisibilityManager::CameraLog()
{
    auto PC = GetWorld()->GetFirstPlayerController();
    FVector CameraLocation;
    FRotator CameraRotation;

    if (!IsValid(PC) && !IsValid(PC->PlayerCameraManager)) return;
    PC->PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotation);
    float FOV = PC->PlayerCameraManager->GetCameraCacheView().FOV;
    float Aspect = PC->PlayerCameraManager->GetCameraCacheView().AspectRatio;

    UE_LOG(LogBDVisibilityManager, Display, TEXT("%s CameraLocation: %s CameraRotation: %s FOV: %f Aspect: %f"),  //
        *PC->GetName(), *CameraLocation.ToCompactString(), *CameraRotation.ToCompactString(), FOV, Aspect);
}
