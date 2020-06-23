#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/CoreUObject/Public/UObject/Object.h>

#include "Runtime/Build/Public/BuildingData/BuildingData.h"
#include "Runtime/Build/Public/Foundation/Foundation.h"
#include "Runtime/Build/Public/FoundationBuilder/PointValidator.h"

#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>
#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagerPtr.h>
#include <Runtime/WorldGrid/Public/GridProjection/GridProjectionInterface.h>
#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

#include "FoundationBuilder.generated.h"

#if !UE_BUILD_SHIPPING
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#endif //!UE_BUILD_SHIPPING

UCLASS(MinimalAPI, ClassGroup = (Build))
class UFoundationBuilder : public UObject
{
	GENERATED_BODY()

public:

	UFoundationBuilder()
		: BuildingData(nullptr)
		, GridSettings(GetDefault<UWorldGridSettings>())
		, PointValidator(nullptr)
		, Callback(nullptr)
		, LastFoundation()
		, GridProjection(*this)
		, AsyncLoader(MakeUnique<FAsyncLoader>())
	{};

	virtual void Init(const UBuildingData& InBuildingData, const TFunction<void(const FFoundation&)>& OnPointsGeneratedCallback)
	{
		BuildingData = &InBuildingData;
		Callback = OnPointsGeneratedCallback;

		if (IGridProjectionInterface* GridProjectionInterface = GridProjection.Get())
		{
			GridProjectionInterface->GetOnRoundedPositionChanged().AddUObject(this, &UFoundationBuilder::UpdateMouseLocation);
			CurrentMouseLocation = GridProjectionInterface->GetRoundedPositionUnderMouse();
		}

		AsyncLoader->RequestLoad<UClass>(BuildingData->BuildingClass, [WeakThis = TWeakObjectPtr<UFoundationBuilder>(this)](UClass& LoadedBuildingClass)
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

		PointValidator = NewObject<UPointValidator>(this);
	};

	virtual void Teardown()
	{
		if (IGridProjectionInterface* GridProjectionInterface = GridProjection.Get())
		{
			GridProjectionInterface->GetOnRoundedPositionChanged().RemoveAll(this);
		}

		if (PointValidator != nullptr)
		{
			PointValidator->Stop();
			PointValidator = nullptr;
		}
	};
	
	void UpdateMouseLocation(const FVector& InNewLocation, const FVector& InOldLocation)
	{
		CurrentMouseLocation = InNewLocation;
		GenerateNewPoints();
	}

	void StartBuild()
	{
		IsCurrentlyBuilding = true;
		StartedLocation = CurrentMouseLocation;
	}

	void EndBuild()
	{
		IsCurrentlyBuilding = false;
	}

	virtual void GenerateNewPoints() PURE_VIRTUAL(UPointBuilder::GenerateNewPoints, );

	virtual void RotateBuild() {};

protected:

	bool IsCurrentlyBuilding;

	UPROPERTY(Transient)
	const UBuildingData* BuildingData;

	UPROPERTY()
	UObject* BuildingCDO;
	
	UPROPERTY(Transient)
	const UWorldGridSettings* GridSettings;

	UPROPERTY()
	UPointValidator* PointValidator;

	TFunction<void(const FFoundation&)> Callback;

	FFoundation LastFoundation;

	TManagerPtr<IGridProjectionInterface> GridProjection;

	FVector StartedLocation;
	FVector CurrentMouseLocation;
	FRotator CurrentRotation;

	TUniquePtr<FAsyncLoader> AsyncLoader;
};