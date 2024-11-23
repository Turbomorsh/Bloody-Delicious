// Bloody Delisious. All Rights Reserved.

#include "AI/BDGroupAIController.h"
#include "AI/BDAICharacter.h"
#include "Framework/BDGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"

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
            InitializeCustomerGroup(CustomersTypeByNum, CustomerControllers, CustomerPawns);
            InitializeCustomerGroup(EvilCustomersTypeByNum, EvilControllers, EvilPawns);
            ActivateRandomCharacter();
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

void ABDGroupAIController::InitializeCustomerGroup(
    const TMap<TSubclassOf<APawn>, int32>& GroupData, TArray<AAIController*>& ControllersArray, TArray<APawn*>& PawnsGroup)
{
    if (!GetWorld()) return;

    UE_LOG(LogBDGroupAIController, Display, TEXT("SpawnCustomers: start creating AI character groups"));

    for (const TPair<TSubclassOf<APawn>, int32>& Pair : GroupData)
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

        UE_LOG(LogBDGroupAIController, Display, TEXT("Creating a group for Pawn class: %s, amount: %d"), *PawnClass->GetName(), NumToSpawn);
        for (int32 i = 0; i < NumToSpawn; ++i)
        {
            FActorSpawnParameters SpawnInfo;
            SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            const auto BDAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

            UE_LOG(LogBDGroupAIController, Display, TEXT("AIController successfully created: %s"), *BDAIController->GetName());

            FActorSpawnParameters SpawnPawnInfo;
            SpawnPawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            const auto BDAICustomer = GetWorld()->SpawnActor<ABDAICharacter>(PawnClass, SpawnLocation, SpawnRotation, SpawnPawnInfo);
            UE_LOG(LogBDGroupAIController, Display, TEXT("AIPawn successfully created: %s"), *BDAICustomer->GetName());

            BDAICustomer->OnCustomerOutside.AddUObject(this, &ThisClass::ActivateRandomCharacter);
            PawnsGroup.Add(BDAICustomer);

            if (BDAIController)
            {
                BDAIController->Possess(BDAICustomer);
                ControllersArray.Add(BDAIController);
            }
        }
    }
}

void ABDGroupAIController::ActivateRandomCharacter()
{
    int32 RandomIndex = FMath::RandRange(0, CustomerPawns.Num() - 1);
    const auto BDAICharacter = Cast<ABDAICharacter>(CustomerPawns[RandomIndex]);
    BDAICharacter->SetCustomerState(EBDCustomerStates::Hungry);
    UE_LOG(LogBDGroupAIController, Display, TEXT("BDAICharacter %s random picked"), *BDAICharacter->GetName());
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