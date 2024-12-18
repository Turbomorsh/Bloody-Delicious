// Bloody Delisious. All Rights Reserved.

#include "AI/BDAICharacter.h"
#include "AI/BDAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactibles/BDBurgerTypeDataAsset.h"
#include "Interactibles/BDFoodTray.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BDInteractionHintWidget.h"
#include "UI/BDGameplayWidget.h"
#include "UI/BDOrderWidget.h"
#include "UI/BDPhraseWidget.h"
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
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    TraySocket = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    if (GetMesh()) TraySocket->SetupAttachment(GetMesh(), "Wirst_L");

    PigHead = CreateDefaultSubobject<UStaticMeshComponent>("PigMesh");
    PigHead->SetupAttachment(GetMesh());
    PigHead->SetVisibility(false);
    PigHead->SetCollisionResponseToAllChannels(ECR_Ignore);

    PhraseWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("PhraseWidgetComponent");
    PhraseWidgetComponent->SetupAttachment(GetRootComponent());
    PhraseWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    PhraseWidgetComponent->SetDrawAtDesiredSize(true);
}

void ABDAICharacter::BeginPlay()
{
    Super::BeginPlay();

    PigHead->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Head_M");

    UE_LOG(LogBDAICharacter, Display, TEXT("%s"), *PhraseWidgetComponent->GetName());

    // set speed
    GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;

    if (GetHorrorManager())
    {
        HorrorManagerPtr = GetHorrorManager();
    }

    InitializeTimers();

    OrderWidget = GetOrderWidget();

    SetFoodText();
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
            SetCustomerState(EBDCustomerStates::OrderAccepted);
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

void ABDAICharacter::Show(TObjectPtr<UObject> InObject)
{
    if (HintWidgetClass && !Hint)
    {
        if (Cast<USceneComponent>(InObject))
        {
            Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
            Hint->AddToViewport();
            Hint->SetText(HintText);
        }
        else if (AnotherHintTrigger)
            if (InObject->IsA(AnotherHintTrigger))
            {
                Hint = CreateWidget<UBDInteractionHintWidget>(GetWorld()->GetFirstPlayerController(), HintWidgetClass);
                Hint->AddToViewport();
                Hint->SetText(AlterHintText);
            }
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

void ABDAICharacter::Scream()
{
    WhisperScream();
    PigScream();
}

void ABDAICharacter::DisableScream()
{
    PigHead->SetVisibility(false);
    if (NormalMaterial) GetMesh()->SetMaterial(0, NormalMaterial);
}

bool ABDAICharacter::PlayDialogue(TArray<FText> InDialogue, int32 Page)
{
    if (Page >= InDialogue.Num() && InDialogue.IsEmpty()) return false;

    if (Page < InDialogue.Num())
    {
        if (InDialogue[Page].ToString().GetCharArray().Num() == 0)
        {
            FFormatOrderedArguments FormatArguments;
            FormatArguments.Add(FText::FromString(TEXT("")));
            FormatArguments.Add(OrderType->OrderName);

            // Leave one Dialogue Array field empty to NPC says his order name
            FText PhraseText = FText::Join(FText::FromString(TEXT("I want ")), FormatArguments);
            SayPhrase(PhraseText);
            DialoguePage++;
        }
        else
        {
            FText PhraseText = InDialogue[Page];
            SayPhrase(PhraseText);
            DialoguePage++;
        }
    }

    if (Page == InDialogue.Num())
    {
        DialoguePage = 0;

        return false;
    }
    return true;
}

void ABDAICharacter::TryGetOrder(TObjectPtr<ABDFoodTray> InOrder)
{
    if (Order == InOrder->GetTray())  // order correct
    {
        UE_LOG(LogBDAICharacter, Display, TEXT("GetTray order correct"));
        InOrder->Grab(TraySocket);
        CurrentFood = InOrder;

        SetCustomerState(EBDCustomerStates::OrderReady);
    }
    else  // order wrong
    {
        FText Phrase = FText::FromString("That's not what I ordered!");
        if (!WrongOrderPhrases.IsEmpty())
        {
            WrongOrderPhrases.Add(Phrase);
            Phrase = WrongOrderPhrases[FMath::RandRange(0, WrongOrderPhrases.Num() - 1)];
        }
        SayPhrase(Phrase);
        UE_LOG(LogBDAICharacter, Display, TEXT("Wrong order"));
    }
}

void ABDAICharacter::Hungry()
{
    bIsEat = false;

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    UE_LOG(LogBDAICharacter, Display, TEXT("%s: I'm so hungry!!!"), *GetName());
}

void ABDAICharacter::Ordering()
{
    if (OrderTypes.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, OrderTypes.Num() - 1);

        SetOrderData(OrderTypes[RandomIndex]);
    }
    UE_LOG(LogBDAICharacter, Display, TEXT("%s"), *OrderType->OrderName.ToString());

    StartCustomerTimer(EBDCustomerTimers::Pending);
}

void ABDAICharacter::OrderAccepted()
{
    StartCustomerTimer(EBDCustomerTimers::Cooking);

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

    //"Ok! I'll wait!"
}

void ABDAICharacter::OrderReady()
{
    /// stop CookingTimer
    CustomerTimerEnd(EBDCustomerTimers::Cooking);
    //"O-o-oh!!! My burger!!!"

    // add HorrorScore
    if (HorrorManagerPtr)
    {
        HorrorManagerPtr->OnOrderScoreChanged.Broadcast(Order.HorrorScore, 0, 0);
    }
    else
    {
        UE_LOG(LogBDAICharacter, Error, TEXT("HorrorManager error"));
    }

    SetCustomerState(EBDCustomerStates::Eating);
}

void ABDAICharacter::Eating()
{
    bIsEat = true;
    //"All correct! Good! Bye!"
    GetWorldTimerManager().SetTimer(EatTimerHandle, this, &ThisClass::EatingTimeOut, TimeToEat, false);
}

void ABDAICharacter::Leaving()
{
    // add FineScore
    if (HorrorManagerPtr && !bIsEat)
    {
        HorrorManagerPtr->OnOrderScoreChanged.Broadcast(0, 0, Order.FineScore);
        if (Order.HorrorScore >= 0)
        {
            HorrorManagerPtr->OnOrderScoreChanged.Broadcast(0, Order.AntiHorrorScore, 0);
        }
    }
    else
    {
        UE_LOG(LogBDAICharacter, Error, TEXT("HorrorManager error"));
    }

    //"Bye!"
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
    CurrentFood->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    CurrentFood = nullptr;

    SetCustomerState(EBDCustomerStates::Leaving);
}

void ABDAICharacter::OnOutside()
{
    SetCustomerState(EBDCustomerStates::None);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    OnCustomerOutside.Broadcast();
    UE_LOG(LogBDAICharacter, Display, TEXT("Customer %s Outside"), *GetName());
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
    SayPhrase(NewState);
}

void ABDAICharacter::SayPhrase(EBDCustomerStates& NewState)
{
    if (NewState == EBDCustomerStates::Leaving && bIsEat) return;
    if (NewState != EBDCustomerStates::Ordering &&    //
        NewState != EBDCustomerStates::OrderReady &&  //
        NewState != EBDCustomerStates::Leaving)
        return;
    const auto PhraseWidget = Cast<UBDPhraseWidget>(PhraseWidgetComponent->GetUserWidgetObject());
    if (!PhraseWidget) return;

    FText Phrase = GetRandomPhraseForState(NewState, PhrasesMap);
    PhraseWidget->SetPhrase(Phrase, VisibleDuration);
}

void ABDAICharacter::SayPhrase(FText& InPhrase)
{
    const auto PhraseWidget = Cast<UBDPhraseWidget>(PhraseWidgetComponent->GetUserWidgetObject());
    if (!PhraseWidget) return;

    PhraseWidget->SetPhrase(InPhrase, VisibleDuration);
}

FText ABDAICharacter::GetRandomPhraseForState(const EBDCustomerStates State, const TMap<EBDCustomerStates, FDialugueData>& InPhrasesMap)
{
    if (const FDialugueData* DialogueData = InPhrasesMap.Find(State))
    {
        const TArray<FText>& Phrases = DialogueData->Phrases;
        if (Phrases.Num() > 0)
        {
            const int32 RandomIndex = FMath::RandRange(0, Phrases.Num() - 1);
            return Phrases[RandomIndex];
        }
    }

    return FText::FromString(TEXT("Hmm-hmm-hmm..."));
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
    if (PigMaterial) GetMesh()->SetMaterial(0, PigMaterial);
}

void ABDAICharacter::WhisperScream()
{
    if (ScreamSound) UGameplayStatics::PlaySoundAtLocation(this, ScreamSound, GetActorLocation());
}

FText ABDAICharacter::GetOrderDescription(const TArray<TEnumAsByte<EFoodType>>& inBurger)
{
    TArray<FText> IngredientsTexts;

    if (true)  // bIsInvese
    {
        for (int32 Index = inBurger.Num() - 1; Index >= 0; --Index)
        {
            const TEnumAsByte<EFoodType>& Food = inBurger[Index];

            FText FoodName = FoodNames[Food];
            IngredientsTexts.Add(FoodName);
        }
    }
    else
    {
        for (const TEnumAsByte<EFoodType>& Food : inBurger)
        {
            FText FoodName = UEnum::GetDisplayValueAsText(Food.GetValue());
            IngredientsTexts.Add(FoodName);
        }
    }

    return FText::Join(FText::FromString("\n"), IngredientsTexts);
}

void ABDAICharacter::SetFoodText()
{
    FoodNames.Add(EFoodType::Cheese, FText::FromString("Cheese"));
    FoodNames.Add(EFoodType::Salad, FText::FromString("Salad"));
    FoodNames.Add(EFoodType::Tomato, FText::FromString("Tomato"));
    FoodNames.Add(EFoodType::Cucumber, FText::FromString("Cucumber"));
    FoodNames.Add(EFoodType::Onion, FText::FromString("Onion"));
    FoodNames.Add(EFoodType::TopBun, FText::FromString("Top Bun"));
    FoodNames.Add(EFoodType::TopBurnedBun, FText::FromString("Top Burned Bun"));
    FoodNames.Add(EFoodType::TopCookedBun, FText::FromString("Top Cooked Bun"));
    FoodNames.Add(EFoodType::Meat, FText::FromString("Patty"));
    FoodNames.Add(EFoodType::CookedMeat, FText::FromString("Cooked Patty"));
    FoodNames.Add(EFoodType::BurnedMeat, FText::FromString("Burned Patty"));
    FoodNames.Add(EFoodType::SauceYellow, FText::FromString("Mustard"));
    FoodNames.Add(EFoodType::SauceRed, FText::FromString("Ketchup"));
    FoodNames.Add(EFoodType::SauceHot, FText::FromString("Chili sauce"));
    FoodNames.Add(EFoodType::SauceWhite, FText::FromString("Mayonnaise"));
    FoodNames.Add(EFoodType::AlterMeat, FText::FromString("SPECIAL Patty"));
    FoodNames.Add(EFoodType::AlterCookedMeat, FText::FromString("SPECIAL Cooked Patty"));
    FoodNames.Add(EFoodType::AlterCheese, FText::FromString("SPECIAL Cheese"));
    FoodNames.Add(EFoodType::BottomBun, FText::FromString("Bottom Bun"));
    FoodNames.Add(EFoodType::BottomBurnedBun, FText::FromString("Bottom Burned Bun"));
    FoodNames.Add(EFoodType::BottomCookedBun, FText::FromString("Bottom Cooked Bun"));
}