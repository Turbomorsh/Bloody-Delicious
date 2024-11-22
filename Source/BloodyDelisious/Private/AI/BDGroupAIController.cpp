// Bloody Delisious. All Rights Reserved.

#include "AI/BDGroupAIController.h"
#include "Framework/BDGameMode.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDGroupAIController, All, All);

ABDGroupAIController::ABDGroupAIController() {}

void ABDGroupAIController::BeginPlay()
{
    Super::BeginPlay();

    const auto GameMode = Cast<ABDGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

void ABDGroupAIController::OnGameStateChanged(EBDGameState State)
{
    switch (State)
    {
        case EBDGameState::Waiting:
            break;
        case EBDGameState::GameInProgress:
            InitializeGroups(CustomersTypeByNum);
            break;
        case EBDGameState::GamePause:
            break;
        case EBDGameState::GameOver:
            break;
        case EBDGameState::GameCompleted:
            break;
        default:
            break;
    }
}

void ABDGroupAIController::InitializeGroups(const TMap<TSubclassOf<APawn>, int32>& GroupData)
{
    if (!GetWorld()) return;

    CustomerGroups.Empty();
    UE_LOG(LogBDGroupAIController, Display, TEXT("SpawnCustomers: start creating AI character groups"));

    for (const TPair<TSubclassOf<APawn>, int32>& Pair : CustomersTypeByNum)
    {
        TSubclassOf<APawn> PawnClass = Pair.Key;
        int32 NumToSpawn = Pair.Value;

        FVector SpawnLocation = FVector::ZeroVector;
        FRotator SpawnRotation = FRotator::ZeroRotator;
        if (GetRandomPlayerStartByTag())
        {
            SpawnLocation = GetRandomPlayerStartByTag()->GetActorLocation();
            SpawnRotation = GetRandomPlayerStartByTag()->GetActorRotation();
        }

        TArray<AAIController*> NewGroup;
        UE_LOG(LogBDGroupAIController, Display, TEXT("Creating a group for Pawn class: %s, amount: %d"), *PawnClass->GetName(), NumToSpawn);
        for (int32 i = 0; i < NumToSpawn; ++i)
        {
            FActorSpawnParameters SpawnInfo;
            SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            const auto BDAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

            UE_LOG(LogBDGroupAIController, Display, TEXT("AIController successfully created: %s"), *BDAIController->GetName());

            FActorSpawnParameters SpawnPawnInfo;
            SpawnPawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            const auto BDAICustomer = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnLocation, SpawnRotation, SpawnInfo);
            UE_LOG(LogBDGroupAIController, Display, TEXT("AIPawn successfully created: %s"), *BDAICustomer->GetName());

            if (BDAIController)
            {
                BDAIController->Possess(BDAICustomer);
                NewGroup.Add(BDAIController);
            }

            APawn* ControlledPawn = BDAIController->GetPawn();
            if (ControlledPawn)
            {
                UE_LOG(LogBDGroupAIController, Display, TEXT("Pawn successfully created: %s, Controller: %s"), *ControlledPawn->GetName(),
                    *BDAIController->GetName());
            }
            else
            {
                UE_LOG(LogBDGroupAIController, Warning, TEXT("Error: Controller %s doesn't control Pawn!"), *BDAIController->GetName());
            }
        }
        CustomerGroups.Add(PawnClass, NewGroup);
        UE_LOG(
            LogBDGroupAIController, Display, TEXT("Group for %s created, amount Controllers: %d"), *PawnClass->GetName(), NewGroup.Num());
    }

    UE_LOG(LogBDGroupAIController, Display, TEXT("All groupd AI characters have been created, amount of types: %d"), CustomerGroups.Num());

    for (const auto& GroupPair : CustomerGroups)
    {
        const TSubclassOf<APawn>& PawnClass = GroupPair.Key;
        const TArray<AAIController*>& Controllers = GroupPair.Value;

        int32 NumControllers = Controllers.Num();
        FString PawnClassName = PawnClass ? PawnClass->GetName() : TEXT("Unknown");
        UE_LOG(LogBDGroupAIController, Display, TEXT("Group for Pawn class: %s contains %d controllers"), *PawnClassName, NumControllers);

        for (const AAIController* Controller : Controllers)
        {
            if (Controller && Controller->GetPawn())
            {
                UE_LOG(LogBDGroupAIController, Display, TEXT(" - Controller: %s, Pawn: %s"), *Controller->GetName(),
                    *Controller->GetPawn()->GetName());
            }
            else
            {
                UE_LOG(LogBDGroupAIController, Warning, TEXT(" - Controller: %s not tied to Pawn!"), *Controller->GetName());
            }
        }
    }
}

void ABDGroupAIController::ActivateRandomCharacter()
{
    //
}

AActor* ABDGroupAIController::GetRandomPlayerStartByTag()
{
    if (!GetWorld()) return nullptr;

    TArray<AActor*> TagPlayerStartActor;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), ToNPCStartTag, TagPlayerStartActor);

    if (TagPlayerStartActor.Num() == 0)
    {
        UE_LOG(LogBDGroupAIController, Warning, TEXT(" - TagPlayerStartActor.Num(): %s !"), *LexToString(TagPlayerStartActor.IsEmpty()));
        return nullptr;
    }

    int32 RandomIndex = FMath::RandRange(0, TagPlayerStartActor.Num() - 1);

    return TagPlayerStartActor[RandomIndex];
}