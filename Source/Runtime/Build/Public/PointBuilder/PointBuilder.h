#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/CoreUObject/Public/UObject/Object.h>

#include "Runtime/Build/Public/BuildingData/BuildingData.h"
#include "Runtime/Build/Public/Foundation/FoundationPoint.h"

#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>
#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagerPtr.h>
#include <Runtime/WorldGrid/Public/GridProjection/GridProjectionInterface.h>
#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

#include "PointBuilder.generated.h"

#if !UE_BUILD_SHIPPING
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#endif //!UE_BUILD_SHIPPING

UCLASS(MinimalAPI, ClassGroup = (Build))
class UPointBuilder : public UObject
{
	GENERATED_BODY()

public:

	UPointBuilder()
		: BuildingData(nullptr)
		, GridSettings(GetDefault<UWorldGridSettings>())
		, Callback(nullptr)
		, GridProjection(*this)
		, AsyncLoader(MakeUnique<FAsyncLoader>())
	{};

	virtual void Init(const UBuildingData& InBuildingData, const TFunction<void(const TArray<FFoundationPoint>&)>& OnPointsGeneratedCallback)
	{
		BuildingData = &InBuildingData;
		Callback = OnPointsGeneratedCallback;

		if (IGridProjectionInterface* GridProjectionInterface = GridProjection.Get())
		{
			GridProjectionInterface->GetOnRoundedPositionChanged().AddUObject(this, &UPointBuilder::UpdateMouseLocation);
			CurrentMouseLocation = GridProjectionInterface->GetRoundedPositionUnderMouse();
		}

		AsyncLoader->RequestLoad<UClass>(BuildingData->BuildingClass, [WeakThis = TWeakObjectPtr<UPointBuilder>(this)](UClass& LoadedBuildingClass)
		{
			if (WeakThis.IsValid())
			{
				if (UObject* LoadedCDO = LoadedBuildingClass.GetDefaultObject())
				{
					WeakThis->BuildingCDO = LoadedCDO;
					WeakThis->GenerateNewPoints();
				}
			}
		});
	};

	virtual void Teardown()
	{
		if (IGridProjectionInterface* GridProjectionInterface = GridProjection.Get())
		{
			GridProjectionInterface->GetOnRoundedPositionChanged().RemoveAll(this);
		}
	};
	
	void UpdateMouseLocation(const FVector& InNewLocation, const FVector& InOldLocation)
	{
		CurrentMouseLocation = InNewLocation;
		GenerateNewPoints();
	}

	void StartBuild()
	{
		FlushPersistentDebugLines(GetWorld());

		StartedLocation = CurrentMouseLocation;

#if !UE_BUILD_SHIPPING
		DrawDebugSphere(GetWorld(), StartedLocation, GridSettings->GridCellSize * 0.5f, 12.f, FColor::Red, false, 20.f);
#endif //!UE_BUILD_SHIPPING
	}

	virtual void GenerateNewPoints() PURE_VIRTUAL(UPointBuilder::GenerateNewPoints, );

protected:

	UPROPERTY(Transient)
	const UBuildingData* BuildingData;

	UPROPERTY()
	UObject* BuildingCDO;
	
	UPROPERTY(Transient)
	const UWorldGridSettings* GridSettings;

	TFunction<void(const TArray<FFoundationPoint>&)> Callback;

	TManagerPtr<IGridProjectionInterface> GridProjection;

	FVector StartedLocation;
	FVector CurrentMouseLocation;

	TUniquePtr<FAsyncLoader> AsyncLoader;
};