#pragma once

#include <Runtime/CoreUObject/Public/UObject/Object.h>

#include "Runtime/Construction/Public/ConstructionInterface.h"

#include "Runtime/Construction/Public/ConstructionRequest.h"

#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>

#include "ConstructionManager.generated.h"

struct FUpdateConstructionRequest;

UCLASS(MinimalAPI)
class UConstructionManager : public UObject
	, public IConstructionInterface
{
	GENERATED_BODY()

public:

	UConstructionManager();

	CONSTRUCTION_API void Init(UWorld& InWorld);

private:

	void BindEvents();
	void OnBuildComplete(const FBuildCompleteEvent& InEv);

	void UpdateRequest(const FUpdateConstructionRequest& InUpdate);

#if !UE_BUILD_SHIPPING
	void UpdateRequests(const float InAmount);
#endif //!UE_BUILD_SHIPPING

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	TArray<FConstructionRequest> Requests;

	TUniquePtr<FAsyncLoader> AsyncLoader;
};