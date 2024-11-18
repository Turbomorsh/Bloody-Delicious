// Bloody Delisious. All Rights Reserved.

#include "AI/BDAICharacter.h"
#include "AI/BDAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Framework/BDOrderManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactibles/BDFoodTray.h"
#include "Rendering/RenderCommandPipes.h"
#include "UI/BDDialogueWidget.h"
#include "UI/BDInteractionHintWidget.h"

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

    TraySocket = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    if (GetMesh()) TraySocket->SetupAttachment(GetMesh(), "Wirst_L");
}

void ABDAICharacter::BeginPlay()
{
    Super::BeginPlay();

    // set speed
    GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;

    TArray<EFoodType> Food;
    Food.Add(EFoodType::Meet);
    Order.Burger = Food;
}

void ABDAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABDAICharacter::Interact(TObjectPtr<UObject> Object)
{
    UE_LOG(LogTemp, Display, TEXT("Interact!"));
    const auto TestObject = Cast<USceneComponent>(Object);
    if (TestObject)
    {
        if (CustomerState == EBDCustomerStates::Ordering)
        {
            UE_LOG(LogTemp, Warning, TEXT("Interact Ordering"));
            PlayDialogue(Dialogue, DialoguePage);
            DialoguePage++;

            // stop PendingTimer
            if (PendingTimerHandle.IsValid() && GetWorldTimerManager().IsTimerActive(PendingTimerHandle))
            {
                GetWorldTimerManager().ClearTimer(PendingTimerHandle);
                UE_LOG(LogTemp, Warning, TEXT("PendingTimer OFF!"));
            }
        }
    }

    if (CurrentFood = Cast<ABDFoodTray>(Object))
    {
        if (CustomerState == EBDCustomerStates::OrderAccepted)
        {
            UE_LOG(LogTemp, Warning, TEXT("Interact OrderAccepted"));
            TryGetOrder(CurrentFood);
        }
    }
}

void ABDAICharacter::Show()
{
    if (HintWidgetClass && !Hint)
    {
        Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
        Hint->AddToViewport();
        Hint->SetText(HintText);
    }
}

void ABDAICharacter::Hide()
{
    if (HintWidgetClass && Hint)
    {
        Hint->RemoveFromParent();
        Hint = nullptr;
    }
}

void ABDAICharacter::PlayDialogue(TArray<FText> InDialogue, int Page)
{
    if (Page <= InDialogue.Num() && InDialogue.Num() != 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayDialogue!"));
        if (DialogueWidgetClass)
        {
            // show DialogueWidget
            // @TODO: fix visible
            if (!DialogueWidget)
            {
                DialogueWidget = CreateWidget<UBDDialogueWidget>(GetWorld()->GetFirstPlayerController(), DialogueWidgetClass);
                DialogueWidget->AddToViewport();
                DialogueWidget->SetVisibility(ESlateVisibility::Visible);
            }
            else if (DialogueWidget->GetVisibility() != ESlateVisibility::Visible)
            {
                DialogueWidget->SetVisibility(ESlateVisibility::Visible);
            }
        }

        if (DialogueWidget && Page < InDialogue.Num())
        {
            DialogueWidget->SetText(InDialogue[Page]);
        }

        if (Page == InDialogue.Num() && DialogueWidget)
        {
            // hide DialogueWidget
            DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
            DialoguePage = 0;
            // DialogueWidget->RemoveFromParent();
            //  Talked = true;

            SetCustomerState(EBDCustomerStates::OrderAccepted);
        }
    }
}

void ABDAICharacter::TryGetOrder(TObjectPtr<ABDFoodTray> InOrder)
{
    if (Order == InOrder->GetTray())
    {
        UE_LOG(LogTemp, Warning, TEXT("GetTray"));
        InOrder->Grab(TraySocket);
        SetCustomerState(EBDCustomerStates::OrderReady);
    }
}

void ABDAICharacter::Hungry()
{
    UE_LOG(LogBDAICharacter, Display, TEXT("I'm so hungry!!!"));
}

void ABDAICharacter::Ordering()
{
    GetWorldTimerManager().SetTimer(PendingTimerHandle, this, &ThisClass::PendingTimeOut, TimeToPendingOrder, false);
    UE_LOG(LogBDAICharacter, Display, TEXT("I would like a burger!"));
    UE_LOG(LogTemp, Warning, TEXT("PendingTimer %f ON!"), TimeToPendingOrder);
}

void ABDAICharacter::OrderAccepted()
{
    GetWorldTimerManager().SetTimer(CookingTimerHandle, this, &ThisClass::CookingTimeOut, TimeToCooking, false);
    UE_LOG(LogBDAICharacter, Display, TEXT("Ok! I'll wait!"));
    UE_LOG(LogTemp, Warning, TEXT("CookingTimer %f ON!"), TimeToCooking);
}

void ABDAICharacter::OrderReady()
{
    /// stop CookingTimer
    GetWorldTimerManager().ClearTimer(CookingTimerHandle);
    UE_LOG(LogTemp, Warning, TEXT("CookingTimer OFF!"));

    UE_LOG(LogBDAICharacter, Display, TEXT("O-o-oh!!! My burger!!!"));
    // check order
    if (IsOrderCorrect())
    {
        SetCustomerState(EBDCustomerStates::Eating);
    }
    else
    {
        UE_LOG(LogBDAICharacter, Display, TEXT("This is the wrong burger! I'm upset!"));
        SetCustomerState(EBDCustomerStates::Leaving);
    }
}

bool ABDAICharacter::IsOrderCorrect()
{
    return true;
}

void ABDAICharacter::Eating()
{
    UE_LOG(LogBDAICharacter, Display, TEXT("All correct! Good! Bye!"));

    GetWorldTimerManager().SetTimer(EatTimerHandle, this, &ThisClass::EatingTimeOut, TimeToEat, false);
}

void ABDAICharacter::Leaving()
{

    GetWorldTimerManager().SetTimer(HungryAgainTimerHandle, this, &ThisClass::HungryAgain, TimeHungryAgain, false);
}

void ABDAICharacter::MakeOrder()
{
    SetCustomerState(EBDCustomerStates::Ordering);
}

void ABDAICharacter::PendingTimeOut()
{
    SetCustomerState(EBDCustomerStates::Leaving);
}

void ABDAICharacter::CookingTimeOut()
{
    SetCustomerState(EBDCustomerStates::Leaving);
}

void ABDAICharacter::EatingTimeOut()
{
    // @TODO: fix Drop
    CurrentFood->Drop(GetActorRotation(), GetActorLocation());
    CurrentFood = nullptr;

    SetCustomerState(EBDCustomerStates::Leaving);
}

void ABDAICharacter::HungryAgain()
{
    SetCustomerState(EBDCustomerStates::Hungry);
}

void ABDAICharacter::TimerUpdate() {}

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

void ABDAICharacter::SetCustomerState(EBDCustomerStates NewState)
{
    CustomerState = NewState;
    SetBlackboardEnumData(CustomerStatusKeyName, CustomerState);

    UE_LOG(LogBDAICharacter, Display, TEXT("%s"), *UEnum::GetValueAsString(CustomerState));

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