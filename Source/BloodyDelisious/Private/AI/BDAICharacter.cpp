// Bloody Delisious. All Rights Reserved.

#include "AI/BDAICharacter.h"
#include "AI/BDAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactibles/BDBurgerTypeDataAsset.h"
#include "Interactibles/BDFoodTray.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BDDialogueWidget.h"
#include "UI/BDInteractionHintWidget.h"
#include "UI/BDGameplayWidget.h"
#include "UI/BDGameHUD.h"

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

    // create dialogue widget
    if (DialogueWidgetClass)
    {
        DialogueWidget = CreateWidget<UBDDialogueWidget>(GetWorld()->GetFirstPlayerController(), DialogueWidgetClass);
        DialogueWidget->AddToViewport();
        DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    // set speed
    GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;

    if (OrderType) Order = OrderType->Order;

    InitializeTimers();
}

void ABDAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABDAICharacter::Interact(TObjectPtr<UObject> Object)
{
    UE_LOG(LogTemp, Display, TEXT("Interact!"));

    if (Cast<USceneComponent>(Object) && CustomerState == EBDCustomerStates::Ordering)
    {
        // stop PendingTimer
        CustomerTimerEnd(EBDCustomerTimers::Pending);

        UE_LOG(LogTemp, Display, TEXT("Interact Ordering"));
        // @TODO: fix play logic
        PlayDialogue(Dialogue, DialoguePage);

        // AcceptOrder
        // SetCustomerState(EBDCustomerStates::OrderAccepted);
    }

    if (TObjectPtr<ABDFoodTray> CastedFood = Cast<ABDFoodTray>(Object))
    {
        CurrentFood = CastedFood;
        if (CustomerState == EBDCustomerStates::OrderAccepted)
        {
            UE_LOG(LogTemp, Display, TEXT("Interact OrderAccepted"));
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

void ABDAICharacter::PlayDialogue(TArray<FText> InDialogue, int32 Page)
{
    if (Page >= InDialogue.Num() && InDialogue.IsEmpty()) return;

    UE_LOG(LogTemp, Display, TEXT("PlayDialogue!"));

    // show DialogueWidget
    // @TODO: fix Play logic
    if (DialogueWidget && Page == 0)
    {
        UE_LOG(LogTemp, Display, TEXT("Page == 0"));
        DialogueWidget->SetVisibility(ESlateVisibility::Visible);
    }

    if (DialogueWidget && Page < InDialogue.Num())
    {
        if (InDialogue[Page].ToString().GetCharArray().Num() == 0)
        {
            FFormatOrderedArguments FormatArguments;
            FormatArguments.Add(FText::FromString(TEXT("")));
            FormatArguments.Add(OrderType->OrderName);

            // Leave one Dialogue Array field empty to NPC says his order name
            DialogueWidget->SetText(FText::Join(FText::FromString(TEXT("I want ")), FormatArguments));
            DialoguePage++;
        }
        else
        {
            DialogueWidget->SetText(InDialogue[Page]);
            DialoguePage++;
            UE_LOG(LogTemp, Display, TEXT("Page %i "), Page);
        }
    }

    if (Page == InDialogue.Num() && DialogueWidget)
    {
        // hide DialogueWidget
        UE_LOG(LogTemp, Display, TEXT("Page %i "), Page);
        DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
        DialoguePage = 0;
        SetCustomerState(EBDCustomerStates::OrderAccepted);
        UE_LOG(LogTemp, Display, TEXT("Page %i "), DialoguePage);
    }
}

void ABDAICharacter::TryGetOrder(TObjectPtr<ABDFoodTray> InOrder)
{
    if (Order == InOrder->GetTray())
    {
        // if order correct
        UE_LOG(LogTemp, Display, TEXT("GetTray"));
        InOrder->Grab(TraySocket);
        CurrentFood = InOrder;

        SetCustomerState(EBDCustomerStates::OrderReady);
    }
}

void ABDAICharacter::Hungry()
{
    UE_LOG(LogBDAICharacter, Display, TEXT("I'm so hungry!!!"));
}

void ABDAICharacter::Ordering()
{
    StartCustomerTimer(EBDCustomerTimers::Pending);

    GetGameplayWidget()->SubscribeToNPCPhrases(this);
    OnCustomerPhraseSay.Broadcast(FText::FromString("I would like a burger!"), true);
}

void ABDAICharacter::OrderAccepted()
{
    StartCustomerTimer(EBDCustomerTimers::Cooking);
    OnCustomerPhraseSay.Broadcast(FText::FromString("Ok! I'll wait!"), true);
}

void ABDAICharacter::OrderReady()
{
    /// stop CookingTimer
    CustomerTimerEnd(EBDCustomerTimers::Cooking);
    OnCustomerPhraseSay.Broadcast(FText::FromString("O-o-oh!!! My burger!!!"), true);

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
    OnCustomerPhraseSay.Broadcast(FText::FromString("All correct! Good! Bye!"), true);
    GetWorldTimerManager().SetTimer(EatTimerHandle, this, &ThisClass::EatingTimeOut, TimeToEat, false);
}

void ABDAICharacter::Leaving()
{
    OnCustomerPhraseSay.Broadcast(FText::FromString("Bye!"), true);
    GetGameplayWidget()->UnSubscribeToNPCPhrases(this);
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
    // CurrentFood->Drop(GetActorRotation(), GetActorLocation());
    CurrentFood->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    CurrentFood = nullptr;

    SetCustomerState(EBDCustomerStates::Leaving);
}

void ABDAICharacter::HungryAgain()
{
    SetCustomerState(EBDCustomerStates::Hungry);
}

UBDGameplayWidget* ABDAICharacter::GetGameplayWidget() const
{
    if (!GetWorld()) return nullptr;

    const auto PlayerController = GetWorld()->GetFirstPlayerController();
    if (!IsValid(PlayerController)) return nullptr;

    const auto HUD = Cast<ABDGameHUD>(PlayerController->GetHUD());
    if (!IsValid(HUD)) return nullptr;

    return Cast<UBDGameplayWidget>(HUD->GetGameplayWidget());
}

void ABDAICharacter::InitializeTimers()
{
    FCustomerTimerData PendingTimerData;
    PendingTimerData.Type = EBDCustomerTimers::Pending;
    PendingTimerData.Duration = TimeToPendingOrder;
    PendingTimerData.TimerDelegate.BindUObject(this, &ThisClass::PendingTimeOut);
    FTimerHandle InPendingTimerHandle;
    PendingTimerData.TimerHandle = InPendingTimerHandle;
    PendingTimerData.ProgressBarUpdateInterval = TimerUpdateInterval;

    FCustomerTimerData CookingTimerData;
    CookingTimerData.Type = EBDCustomerTimers::Cooking;
    CookingTimerData.Duration = TimeToCooking;
    CookingTimerData.TimerDelegate.BindUObject(this, &ThisClass::CookingTimeOut);
    FTimerHandle InCookingTimerHandle;
    CookingTimerData.TimerHandle = InCookingTimerHandle;
    CookingTimerData.ProgressBarUpdateInterval = TimerUpdateInterval;

    CustomerTimersMap.Add(EBDCustomerTimers::Pending, PendingTimerData);
    CustomerTimersMap.Add(EBDCustomerTimers::Cooking, CookingTimerData);
}

void ABDAICharacter::StartCustomerTimer(EBDCustomerTimers InETimer)
{
    if (CustomerTimersMap.Contains(InETimer))
    {
        GetGameplayWidget()->SubscribeToNPCTimers(this);
        FCustomerTimerData& ETimerData = CustomerTimersMap[InETimer];

        GetWorldTimerManager().SetTimer(  //
            ETimerData.TimerHandle,       //
            ETimerData.TimerDelegate,     //
            ETimerData.Duration,          //
            false);

        ETimerData.ProgressBarTimerDelegate.BindUObject(  //
            this,                                         //
            &ThisClass::UpdateProgressBar,                //
            ETimerData.Type,                              //
            ETimerData.Duration);

        GetWorldTimerManager().SetTimer(           //
            ETimerData.ProgressBarTimerHandle,     //
            ETimerData.ProgressBarTimerDelegate,   //
            ETimerData.ProgressBarUpdateInterval,  //
            true);

        ETimerData.OnMainTimerClearDelegate.BindUObject(this, &ThisClass::CustomerTimerEnd, InETimer);

        UE_LOG(LogBDAICharacter, Display, TEXT("%s timer STARTED"), *UEnum::GetValueAsString(InETimer));
    }
}

void ABDAICharacter::UpdateProgressBar(EBDCustomerTimers InETimer, float TotalTime)
{
    if (!GetWorld()) return;

    if (CustomerTimersMap.Contains(InETimer))
    {
        FCustomerTimerData& ETimerData = CustomerTimersMap[InETimer];

        float InTimeRemaining = GetWorldTimerManager().GetTimerRemaining(ETimerData.TimerHandle);

        OnCustomerTimerChanged.Broadcast(InTimeRemaining / TotalTime, ETimerData.Type);

        if (InTimeRemaining <= 0.0f)
        {
            GetWorldTimerManager().ClearTimer(ETimerData.ProgressBarTimerHandle);
            UE_LOG(LogBDAICharacter, Display, TEXT("ProgressBar %s stoped"), *UEnum::GetValueAsString(InETimer));
        }
    }
}

void ABDAICharacter::CustomerTimerEnd(EBDCustomerTimers InETimer)
{
    if (!CustomerTimersMap.Contains(InETimer)) return;

    GetGameplayWidget()->UnSubscribeFromNPCTimers(this);

    FCustomerTimerData& ETimerData = CustomerTimersMap[InETimer];

    GetWorldTimerManager().ClearTimer(ETimerData.TimerHandle);

    GetWorldTimerManager().ClearTimer(ETimerData.ProgressBarTimerHandle);

    UE_LOG(LogBDAICharacter, Display, TEXT("%s timer stoped"), *UEnum::GetValueAsString(InETimer));
};

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
void ABDAICharacter::SetOrderData(TObjectPtr<UBDBurgerTypeDataAsset> InOrder)
{
    if (InOrder) OrderType = InOrder;
}
void ABDAICharacter::Scream()
{
    if (ScreamSound) UGameplayStatics::PlaySoundAtLocation(this, ScreamSound, GetActorLocation());
}