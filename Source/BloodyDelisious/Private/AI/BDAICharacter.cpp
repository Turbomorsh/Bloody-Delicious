// Bloody Delisious. All Rights Reserved.

#include "AI/BDAICharacter.h"
#include "AI/BDAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactibles/BDBurgerTypeDataAsset.h"
#include "Interactibles/BDFoodTray.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BDDialogueWidget.h"
#include "UI/BDInteractionHintWidget.h"
#include "UI/BDGameplayWidget.h"
#include "UI/BDOrderWidget.h"
#include "UI/BDGameHUD.h"
#include "Framework/BDHorrorManager.h"
#include "Framework/BDGameMode.h"
#include "EngineUtils.h"
#include "Components/WidgetComponent.h"
#include "Framework/BDCoreTypes.h"

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

    PigHead = CreateDefaultSubobject<UStaticMeshComponent>("PigMesh");
    PigHead->SetupAttachment(GetMesh());
    PigHead->SetVisibility(false);
}
void ABDAICharacter::PostInitProperties()
{
    Super::PostInitProperties();
    PigHead->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Head_M");
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

    if (GetHorrorManager())
    {
        HorrorManagerPtr = GetHorrorManager();
    }

    InitializeTimers();

    OrderWidget = GetOrderWidget();
}

UBDHorrorManager* ABDAICharacter::GetHorrorManager()
{
    if (!GetWorld()) return nullptr;

    const auto GameMode = Cast<ABDGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return nullptr;

    const auto HorrorManager = GameMode->GetHorrorManager();
    if (!HorrorManager) return nullptr;

    return HorrorManager;
}

void ABDAICharacter::Interact(TObjectPtr<UObject> Object)
{
    const auto CastetScene = Cast<USceneComponent>(Object);
    if (CastetScene && CustomerState == EBDCustomerStates::Ordering)
    {
        // stop PendingTimer
        CustomerTimerEnd(EBDCustomerTimers::Pending);

        const auto Character = Cast<ACharacter>(CastetScene->GetOwner());
        if (PlayDialogue(Dialogue, DialoguePage))
        {
            UE_LOG(LogBDAICharacter, Display, TEXT("PlayDialogue"));
            if (Character)
            {
                Character->GetCharacterMovement()->DisableMovement();
            }
        }
        else
        {
            UE_LOG(LogBDAICharacter, Display, TEXT("Dialogue over"));
            Character->GetCharacterMovement()->SetDefaultMovementMode();
        }
    }

    if (TObjectPtr<ABDFoodTray> CastedFood = Cast<ABDFoodTray>(Object))
    {
        CurrentFood = CastedFood;
        if (CustomerState == EBDCustomerStates::OrderAccepted)
        {
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
bool ABDAICharacter::Scream(int32 HorrorValue)
{
    if (HorrorValue >= WhisperScreamCost && HorrorValue < PigScreamCost)
    {
        WhisperScream();
        return true;
    }
    else if (HorrorValue >= PigScreamCost)
    {
        PigScream();
        return true;
    }
    else
    {
        return false;
    }
}

bool ABDAICharacter::PlayDialogue(TArray<FText> InDialogue, int32 Page)
{
    if (Page >= InDialogue.Num() && InDialogue.IsEmpty()) return false;

    // show DialogueWidget
    if (DialogueWidget && Page == 0)
    {
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
        }
    }

    if (Page == InDialogue.Num() && DialogueWidget)
    {
        // hide DialogueWidget

        DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
        DialoguePage = 0;
        SetCustomerState(EBDCustomerStates::OrderAccepted);

        return false;
    }
    return true;
}

void ABDAICharacter::TryGetOrder(TObjectPtr<ABDFoodTray> InOrder)
{
    if (Order == InOrder->GetTray())
    {
        // if order correct
        UE_LOG(LogBDAICharacter, Display, TEXT("GetTray"));
        InOrder->Grab(TraySocket);
        CurrentFood = InOrder;

        SetCustomerState(EBDCustomerStates::OrderReady);
    }
    else
    {
        OnCustomerPhraseSay.Broadcast(FText::FromString("That's not what I ordered."), true);
        UE_LOG(LogBDAICharacter, Display, TEXT("Wrong order"));
    }
}

void ABDAICharacter::Hungry()
{
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    UE_LOG(LogBDAICharacter, Display, TEXT("%s: I'm so hungry!!!"), *this->GetName());
}

void ABDAICharacter::Ordering()
{
    if (OrderTypes.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, OrderTypes.Num() - 1);

        SetOrderData(OrderTypes[RandomIndex]);
    }
    UE_LOG(LogBDAICharacter, Display, TEXT("%s"), *OrderType->OrderName.ToString());

    // order widget subscribe
    if (OrderWidget)
    {
        OrderWidget->OnOrderText.Broadcast(OrderType->OrderName, GetOrderDescription(Order.Burger));
        OnCustomerStateChanged.AddUObject(OrderWidget, &UBDOrderWidget::HandleVisibility);
    }
    else
    {
        UE_LOG(LogBDAICharacter, Warning, TEXT("OrderWidget isn't setup!"));
    }

    StartCustomerTimer(EBDCustomerTimers::Pending);

    GetGameplayWidget()->SubscribeToNPCPhrases(this);
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

    // add 2 points

    if (HorrorManagerPtr)
    {
        HorrorManagerPtr->OnSubmissionScoreChanged.Broadcast(Score);
    }
    else
    {
        UE_LOG(LogBDAICharacter, Error, TEXT("HorrorManager error"));
    }

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
    // GetWorldTimerManager().SetTimer(HungryAgainTimerHandle, this, &ThisClass::HungryAgain, TimeHungryAgain, false);
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

void ABDAICharacter::OnOutside()
{
    SetCustomerState(EBDCustomerStates::None);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    OnCustomerOutside.Broadcast();
    UE_LOG(LogBDAICharacter, Warning, TEXT("Customer %s Outside"), *this->GetName());
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

UBDOrderWidget* ABDAICharacter::GetOrderWidget() const
{
    if (!GetWorld()) return nullptr;

    AActor* WidgetActor = nullptr;
    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        if (It->ActorHasTag(OrderTag))
        {
            WidgetActor = *It;
            break;
        }
    }

    if (WidgetActor)
    {
        const auto WidgetComp = WidgetActor->FindComponentByClass<UWidgetComponent>();
        if (WidgetComp)
        {
            return Cast<UBDOrderWidget>(WidgetComp->GetUserWidgetObject());
        }
    }

    return nullptr;
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
    UE_LOG(LogBDAICharacter, Display, TEXT("%s %s"), *this->GetName(), *UEnum::GetValueAsString(CustomerState));

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
    OnCustomerStateChanged.Broadcast(CustomerState);
}

bool ABDAICharacter::IsWaiting() const
{
    bool bIsWait = GetCustomerState() == EBDCustomerStates::OrderAccepted;
    return bIsWait;
}

void ABDAICharacter::SetOrderData(TObjectPtr<UBDBurgerTypeDataAsset> InOrder)
{
    if (InOrder) OrderType = InOrder;
    if (OrderType) Order = OrderType->Order;
}
void ABDAICharacter::PigScream()
{
    PigHead->SetVisibility(true);
}
void ABDAICharacter::WhisperScream()
{
    if (ScreamSound) UGameplayStatics::PlaySoundAtLocation(this, ScreamSound, GetActorLocation());
}

FText ABDAICharacter::GetOrderDescription(const TArray<TEnumAsByte<EFoodType>>& inBurger)
{
    TArray<FText> IngredientsTexts;

    for (const TEnumAsByte<EFoodType>& Food : inBurger)
    {
        FText FoodName = UEnum::GetDisplayValueAsText(Food.GetValue());
        IngredientsTexts.Add(FoodName);
    }

    return FText::Join(FText::FromString("\n"), IngredientsTexts);
}