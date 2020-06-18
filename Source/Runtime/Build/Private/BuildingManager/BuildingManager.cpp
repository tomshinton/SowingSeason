#include "Runtime/Build/Public/BuildingManager/BuildingManager.h"
#include "Runtime/Build/Public/Events/BuildEvents.h"

#include "ObjectMessaging/Public/Listener/ObjectMessagingListenerInterface.h"

DEFINE_LOG_CATEGORY_STATIC(BuildingManager, Log, Log)

void UBuildingManager::Init(const UWorld& InWorld)
{
	BindEvents();
}

void UBuildingManager::BindEvents()
{
	if (IObjectMessagingListenerInterface* MessagingListener = Cast<IObjectMessagingListenerInterface>(GetWorld()->GetGameInstance()))
	{
		TWeakObjectPtr<UBuildingManager> WeakThis = TWeakObjectPtr<UBuildingManager>(this);
		MessagingListener->GetListener().Bind<FBuildCompleteEvent>([WeakThis](const FBuildCompleteEvent& InEv)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->OnBuildingComplete(InEv);
			}
		});
	}
}

void UBuildingManager::OnBuildingComplete(const FBuildCompleteEvent& InEv)
{
	UE_LOG(BuildingManager, Log, TEXT("Building complete"));
}

void UBuildingManager::TrySpawnSingleBuilding(const UBuildingData& InData, const FFoundation& InFoundation)
{

}
