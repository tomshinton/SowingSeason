#include "Runtime/Construction/Public/ConstructionManager.h"

#include "Runtime/Construction/Public/Events/ConstructionEvents.h"

#include <Runtime/Build/Public/BuildingData/BuildingData.h>
#include <Runtime/Build/Public/Events/BuildEvents.h>
#include <ObjectMessaging/Public/Listener/ObjectMessagingListenerInterface.h>
#include <ObjectMessaging/Public/Sender/ObjectMessagingFunctions.h>

UConstructionManager::UConstructionManager()
	: World(nullptr)
	, Requests()
	, AsyncLoader(MakeUnique<FAsyncLoader>())
{

}

void UConstructionManager::Init(UWorld& InWorld)
{
	World = &InWorld;

	BindEvents();
}

void UConstructionManager::BindEvents()
{
	if (IObjectMessagingListenerInterface* MessagingListener = Cast<IObjectMessagingListenerInterface>(GetWorld()->GetGameInstance()))
	{
		TWeakObjectPtr<UConstructionManager> WeakThis = TWeakObjectPtr<UConstructionManager>(this);
		
		MessagingListener->GetListener().Bind<FBuildCompleteEvent>([WeakThis](const FBuildCompleteEvent& InEv)
		{
			if (WeakThis.IsValid() && InEv.BuildingFoundation.IsValidFoundation())
			{
				WeakThis->OnBuildComplete(InEv);
			}
		});

		MessagingListener->GetListener().Bind<FUpdateConstructionRequest>([WeakThis](const FUpdateConstructionRequest& InEv)
		{
			if (WeakThis.IsValid())
			{
				if (InEv.ID.IsValid())
				{
					WeakThis->UpdateRequest(InEv);
				}
#if !UE_BUILD_SHIPPING
				else
				{
					WeakThis->UpdateRequests(InEv.Amount);
				}
#endif //!UE_BUILD_SHIPPING
			}
		});
	}
}

void UConstructionManager::OnBuildComplete(const FBuildCompleteEvent& InEv)
{
	AsyncLoader->RequestLoad<UConstructionData>(InEv.BuildingData->ConstructionData, [WeakThis = TWeakObjectPtr<UConstructionManager>(this), InEv](UConstructionData& LoadedData)
	{
		if (WeakThis.IsValid())
		{
			FConstructionRequest NewRequest(InEv, LoadedData);
			NewRequest.TargetProgress = LoadedData.GetTimeForState(NewRequest.CurrentState);

			WeakThis->Requests.Add(NewRequest);
		}
	});
}

void UConstructionManager::UpdateRequest(const FUpdateConstructionRequest& InUpdate)
{

}

#if !UE_BUILD_SHIPPING
void UConstructionManager::UpdateRequests(const float InAmount)
{
	for (int32 i = Requests.Num() - 1; i >= 0; --i)
	{
		FConstructionRequest& Request = Requests[i];
		Request += InAmount;

		if (Request.CurrentState == EConstructionState::Finished)
		{
			ObjectMessagingFunctions::SendMessage<FConstructionCompleteEvent>(*GetWorld()->GetGameInstance(), FConstructionCompleteEvent(Request.Source));
			Requests.RemoveAt(i);
		}
	}
}
#endif //UE_BUILD_SHIPPING
