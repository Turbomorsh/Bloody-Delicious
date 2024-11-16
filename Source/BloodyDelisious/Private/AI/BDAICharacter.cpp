// Bloody Delisious. All Rights Reserved.

#include "AI/BDAICharacter.h"
#include "AI/BDAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Framework/BDOrderManager.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDAICharacter, All, All);

ABDAICharacter::ABDAICharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ABDAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void ABDAICharacter::BeginPlay()
{
    Super::BeginPlay();

    // set speed
    GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}

void ABDAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABDAICharacter::Hungry()
{
    SetBlackboardEnumData(CustomerStatusKeyName, CustomerState);

    UE_LOG(LogBDAICharacter, Display, TEXT("I'm so hungry!!!"));
}

void ABDAICharacter::Ordering()
{
    UE_LOG(LogBDAICharacter, Display, TEXT("I would like a burger!"));
}

void ABDAICharacter::OrderAccepted()
{
    UE_LOG(LogBDAICharacter, Display, TEXT("Ok! I'll wait!"));
}

void ABDAICharacter::OrderReady()
{
    if (!OrderManager)
    {
        UE_LOG(LogBDAICharacter, Warning, TEXT("OrderManager faild"));
        return;
    }

    OrderManager->SetOrderState(EBDOrderStates::NextInLine);

    // for test all orders correct
    UE_LOG(LogBDAICharacter, Display, TEXT("Is it my order? All correct! Good! Bye!"));
    SetCustomerState(EBDCustomerStates::Eating);
}

void ABDAICharacter::Eating()
{
    SetBlackboardEnumData(CustomerStatusKeyName, CustomerState);

    GetWorldTimerManager().SetTimer(HungryAgainTimerHandle, this, &ThisClass::HungryAgain, TimeHungryAgain, false);
}

void ABDAICharacter::Leaving()
{
    SetBlackboardEnumData(CustomerStatusKeyName, CustomerState);

    GetWorldTimerManager().SetTimer(HungryAgainTimerHandle, this, &ThisClass::HungryAgain, TimeHungryAgain, false);
}

void ABDAICharacter::SetBlackboardEnumData(FName KeyName, EBDCustomerStates& NewState)
{
    const auto BDController = Cast<AAIController>(Controller);
    if (!BDController)
    {
        UE_LOG(LogBDAICharacter, Warning, TEXT("AAIController faild!"));
        return;
    }

    auto BlackboardComp = BDController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogBDAICharacter, Warning, TEXT("BlackboardComponent faild!"));
        return;
    }

    FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (!BlackboardComp->IsValidKey(KeyID))
    {
        UE_LOG(LogBDAICharacter, Warning, TEXT("BlackboardComponent faild!"));
        return;
    }

    BDController->GetBlackboardComponent()->SetValueAsEnum(CustomerStatusKeyName, static_cast<uint8>(NewState));
}

void ABDAICharacter::HungryAgain()
{
    SetCustomerState(EBDCustomerStates::Hungry);
}

void ABDAICharacter::SetCustomerState(EBDCustomerStates NewState)
{
    CustomerState = NewState;

    UE_LOG(LogTemp, Display, TEXT("%s"), *UEnum::GetValueAsString(CustomerState));

    switch (CustomerState)
    {
        case EBDCustomerStates::Hungry:
            Hungry();
            break;
        case EBDCustomerStates::Ordering:
            Ordering();
            break;
        case EBDCustomerStates::OrderAccepted:
            OrderAccepted();
            break;
        case EBDCustomerStates::OrderReady:
            OrderReady();
            break;
        case EBDCustomerStates::Eating:
            Eating();
            break;
        case EBDCustomerStates::Leaving:
            Leaving();
            break;
        default:
            break;
    }
}