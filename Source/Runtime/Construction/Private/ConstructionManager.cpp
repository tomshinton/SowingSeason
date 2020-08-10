// Sowing Season - Tom Shinton 2020

#include "Runtime/Construction/Public/ConstructionManager.h"

#include "Runtime/Construction/Public/ConstructionSite/ConstructionSite.h"
#include "Runtime/Construction/Public/Events/ConstructionEvents.h"

#include <Runtime/Build/Public/BuildSettings.h>
#include <Runtime/Build/Public/BuildingData/BuildingData.h>
#include <Runtime/Build/Public/Events/BuildEvents.h>
#include <ObjectMessaging/Public/Listener/ObjectMessagingListenerInterface.h>
#include <ObjectMessaging/Public/Sender/ObjectMessagingFunctions.h>

UConstructionManager::UConstructionManager()
	: World(nullptr)
	, BuildSettings(GetDefault<UBuildSettings>())
	, Requests()
	, AsyncLoader(MakeUnique<FAsyncLoader>())
{

}

void UConstructionManager::Init(UWorld& InWorld)
{
	World = &InWorld;

	BindEvents();
	LoadAssets();
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

void UConstructionManager::LoadAssets()
{
	AsyncLoader->RequestLoad<UClass>(BuildSettings->ConstructionSiteClass, [WeakThis = TWeakObjectPtr<UConstructionManager>(this)](UClass& LoadedClass)
	{
		if (WeakThis.IsValid())
		{
			WeakThis->ConstructionSiteClass = &LoadedClass;
		}
	});
}

void UConstructionManager::OnBuildComplete(const FBuildCompleteEvent& InEv)
{
	AsyncLoader->RequestLoad<UConstructionData>(InEv.BuildingData->ConstructionData, [WeakThis = TWeakObjectPtr<UConstructionManager>(this), InEv](UConstructionData& LoadedData)
	{
		if (WeakThis.IsValid())
		{
			switch (InEv.BuildingData->BuildMode)
			{
			case EBuildMode::Grid:
				break;
			case EBuildMode::Linear:
				for (const FFoundationPoint& InPoint : InEv.BuildingFoundation.Points)
				{
					FBuildCompleteEvent Ev = InEv;
					WeakThis->GenerateLinearRequest(Ev, InPoint);

					WeakThis->CreateRequest(Ev, LoadedData);
				}
				break;
			case EBuildMode::Single:
				WeakThis->CreateRequest(InEv, LoadedData);
				break;
			}
		}
	});
}

void UConstructionManager::CreateRequest(const FBuildCompleteEvent& InBuildCompleteEvent, UConstructionData& InLoadedData)
{
	TSharedPtr<FConstructionRequest> NewRequest = MakeShareable(new FConstructionRequest(InBuildCompleteEvent, InLoadedData));

	if (AConstructionSite* ConstructionSite = GetWorld()->SpawnActor<AConstructionSite>(ConstructionSiteClass, InBuildCompleteEvent.BuildingFoundation.Transform()))
	{
		NewRequest->SpawnedConstructionSite = ConstructionSite;

		const int32 NewRequestIndex = Requests.Add(MoveTemp(NewRequest));
		ConstructionSite->SetConstructionRequest(Requests[NewRequestIndex]);
	}
}

void UConstructionManager::GenerateLinearRequest(FBuildCompleteEvent& InEv, const FFoundationPoint& InSourcePoint) const
{
	InEv.BuildingFoundation.Points.Empty();
	InEv.BuildingFoundation.Points.Add(InSourcePoint);
	InEv.BuildingFoundation.Location = InSourcePoint.Location;
}

void UConstructionManager::UpdateRequest(const FUpdateConstructionRequest& InUpdate)
{

}

#if !UE_BUILD_SHIPPING
void UConstructionManager::UpdateRequests(const float InAmount)
{
	for (int32 i = Requests.Num() - 1; i >= 0; --i)
	{
		TSharedPtr<FConstructionRequest> Request = Requests[i];
		Request->Advance(InAmount);

		if (Request->CurrentState == EConstructionState::Finished)
		{
			ObjectMessagingFunctions::SendMessage<FConstructionCompleteEvent>(*GetWorld()->GetGameInstance(), FConstructionCompleteEvent(Request->Source));
			Requests[i]->Complete();
			Requests.RemoveAt(i);
		}
	}
}
#endif //UE_BUILD_SHIPPING


