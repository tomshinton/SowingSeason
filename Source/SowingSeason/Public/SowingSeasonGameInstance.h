// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/Engine/GameInstance.h>

#include <ObjectMessaging/Public/Listener/ObjectMessagingListenerInterface.h>

#include "SowingSeasonGameInstance.generated.h"

UCLASS(MinimalAPI)
class USowingSeasonGameInstance : public UGameInstance
	, public IObjectMessagingListenerInterface
{
	GENERATED_BODY()

private:

	//IObjectMessagingListenerInterface
	FObjectMessagingListener& GetListener() override { return MessageListener; }
	//~IObjectMessagingListenerInterface

	FObjectMessagingListener MessageListener;
};
