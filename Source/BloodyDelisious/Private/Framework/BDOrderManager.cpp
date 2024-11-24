// Bloody Delisious. All Rights Reserved.

#include "Framework/BDOrderManager.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "AI/BDAICharacter.h"
#include "Framework/BDHorrorManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDOrderManager, All, All);

ABDOrderManager::ABDOrderManager()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    OrderingAriaCollision = CreateDefaultSubobject<UBoxComponent>("OrderingAriaCollision");
    OrderingAriaCollision->InitBoxExtent(FVector(50.0f));
    OrderingAriaCollision->SetupAttachment(GetRootComponent());
    OrderingAriaCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OrderingAriaBeginOverlap);

    AcceptanceAriaCollision = CreateDefaultSubobject<UBoxComponent>("AcceptanceAriaCollision");
    AcceptanceAriaCollision->InitBoxExtent(FVector(50.0f));
    AcceptanceAriaCollision->SetupAttachment(GetRootComponent());
    AcceptanceAriaCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AcceptanceAriaBeginOverlap);

    CookingAriaCollision = CreateDefaultSubobject<UBoxComponent>("CookingAriaCollision");
    CookingAriaCollision->InitBoxExtent(FVector(50.0f));
    CookingAriaCollision->SetupAttachment(GetRootComponent());
    CookingAriaCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::CookingAriaBeginOverlap);

    DeliveryAriaCollision = CreateDefaultSubobject<UBoxComponent>("DeliveryAriaCollision");
    DeliveryAriaCollision->InitBoxExtent(FVector(50.0f));
    DeliveryAriaCollision->SetupAttachment(GetRootComponent());
    DeliveryAriaCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::DeliveryAriaBeginOverlap);

    OrderingAriaWidget = CreateDefaultSubobject<UWidgetComponent>("OrderingWidgetComponent");
    OrderingAriaWidget->SetupAttachment(OrderingAriaCollision);
    AcceptanceAriaWidget = CreateDefaultSubobject<UWidgetComponent>("AcceptanceWidgetComponent");
    AcceptanceAriaWidget->SetupAttachment(AcceptanceAriaCollision);
    CookingAriaWidget = CreateDefaultSubobject<UWidgetComponent>("CookingWidgetComponent");
    CookingAriaWidget->SetupAttachment(CookingAriaCollision);
    DeliveryAriaWidget = CreateDefaultSubobject<UWidgetComponent>("DeliveryWidgetComponent");
    DeliveryAriaWidget->SetupAttachment(DeliveryAriaCollision);
}

void ABDOrderManager::BeginPlay()
{
    Super::BeginPlay();
}

void ABDOrderManager::MakeOrder()
{
    // CurrentCustomer->SetOrderManagerPtr(this);
    // CurrentCustomer->SetCustomerState(EBDCustomerStates::Ordering);
    OrderState = EBDOrderStates::Pending;

    GetWorldTimerManager().SetTimer(PendingTimerHandle, this, &ThisClass::PendingTimeOut, TimeToPendingOrder, false);
    bool Active = GetWorldTimerManager().IsTimerActive(PendingTimerHandle);
}

void ABDOrderManager::ReceiveOrder()
{
    // CurrentCustomer->SetCustomerState(EBDCustomerStates::OrderAccepted);
    OrderState = EBDOrderStates::Cooking;

    GetWorldTimerManager().ClearTimer(PendingTimerHandle);
    GetWorldTimerManager().SetTimer(CookingTimerHandle, this, &ThisClass::CookingTimeOut, TimeToCooking, false);
}

void ABDOrderManager::CookOrder()
{
    OrderState = EBDOrderStates::Ready;
}

void ABDOrderManager::DeliverOrder()
{
    GetWorldTimerManager().ClearTimer(CookingTimerHandle);
    OrderState = EBDOrderStates::NextInLine;

    // CurrentCustomer->SetCustomerState(EBDCustomerStates::OrderReady);

    // if (IsOrderCorrect())
    //{
    //     CurrentCustomer->SetCustomerState(EBDCustomerStates::Eating);
    // }
    // else
    //{
    //     CurrentCustomer->SetCustomerState(EBDCustomerStates::Leaving);
    // }
}

void ABDOrderManager::PendingTimeOut()
{
    // CurrentCustomer->SetCustomerState(EBDCustomerStates::Leaving);
    OrderState = EBDOrderStates::NextInLine;

    UE_LOG(LogBDOrderManager, Display, TEXT("PendingTimeOut"));
}

void ABDOrderManager::CookingTimeOut()
{
    // CurrentCustomer->SetCustomerState(EBDCustomerStates::Leaving);
    OrderState = EBDOrderStates::NextInLine;

    UE_LOG(LogBDOrderManager, Display, TEXT("CookingTimeOut"));
}

bool ABDOrderManager::IsOrderCorrect()
{
    // check order
    return true;
}

bool ABDOrderManager::IsPlayerCharacter(AActor* OtherActor)
{
    if (OtherActor == nullptr || !PlayerClass)
    {
        UE_LOG(LogBDOrderManager, Warning, TEXT("OtherActor or PlayerClass nullptr!"));
        return false;
    }

    if (!OtherActor->IsA(PlayerClass))
    {
        UE_LOG(LogBDOrderManager, Warning, TEXT("OtherActor is not of type PlayerClass!"));
        return false;
    }
    return true;
}

void ABDOrderManager::OrderingAriaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogBDOrderManager, Display, TEXT("OrderingAriaBeginOverlap"));

    const auto BDAICharacter = Cast<ABDAICharacter>(OtherActor);
    if (!BDAICharacter) return;

    // CurrentCustomer = BDAICharacter;

    /* switch (CurrentCustomer->GetCustomerState())
     {
         case EBDCustomerStates::Hungry:
             MakeOrder();
             break;
         case EBDCustomerStates::OrderReady:
             DeliverOrder();
             break;
         default:
             break;
     }*/
}

void ABDOrderManager::AcceptanceAriaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!IsPlayerCharacter(OtherActor)) return;
    UE_LOG(LogBDOrderManager, Display, TEXT("AcceptanceAriaBeginOverlap"));

    if (OrderState == EBDOrderStates::Pending)
    {
        ReceiveOrder();
    }
    else
    {
        UE_LOG(LogBDOrderManager, Display, TEXT("No orders!"));
    }
}

void ABDOrderManager::CookingAriaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!IsPlayerCharacter(OtherActor)) return;
    UE_LOG(LogBDOrderManager, Display, TEXT("CookingAriaBeginOverlap"));

    if (OrderState == EBDOrderStates::Cooking)
    {
        CookOrder();
    }
    else
    {
        UE_LOG(LogBDOrderManager, Display, TEXT("Order not accept!"));
    }
}

void ABDOrderManager::DeliveryAriaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!IsPlayerCharacter(OtherActor)) return;
    UE_LOG(LogBDOrderManager, Display, TEXT("DeliveryAriaBeginOverlap"));

    if (OrderState == EBDOrderStates::Ready)
    {
        DeliverOrder();
    }
    else
    {
        UE_LOG(LogBDOrderManager, Display, TEXT("Not redy yet!"));
    }
}
