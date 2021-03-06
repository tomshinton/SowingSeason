// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/CoreUObject/Public/UObject/Object.h>

#include "Runtime/Build/Public/Foundation/Foundation.h"
#include "Runtime/Build/Public/FoundationBuilder/PointPolicy/PointOccludedPointPolicy.h"
#include "Runtime/Build/Public/FoundationBuilder/PointPolicy/PointValidationPolicy.h"
#include "Runtime/Build/Public/FoundationBuilder/PointPolicy/TooSteepPointPolicy.h"

#include <Runtime/Engine/Public/WorldCollision.h>
#include <Runtime/Core/Public/Templates/SharedPointer.h>
#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

#include "PointValidator.generated.h"

DEFINE_LOG_CATEGORY_STATIC(PointValidationTaskLog, Log, Log)

UCLASS()
class UPointValidator : public UObject
{
	GENERATED_BODY()

public:

	UPointValidator()
		: Points()
		, World(nullptr)
		, Callback(nullptr)
		, GridSettings(GetDefault<UWorldGridSettings>())
		, ValidationDelegate()
		, NumCompletedTraces(0)
	{
		ValidationDelegate.BindWeakLambda(this, [WeakThis = TWeakObjectPtr<UPointValidator>(this)](const FTraceHandle& Handle, FTraceDatum& Data)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->OnTraceComplete(Handle, Data);
			}
		});
	};

	void Run(const TArray<FFoundationPoint>& InPoints, UWorld& InWorld, const TFunction<void(const TArray<FFoundationPoint>&)>& InCallback)
	{

		Requests.Empty();

		Points = InPoints;
		World = &InWorld;
		Callback = InCallback;

		Requests.Reserve(Points.Num());

		if (Policies.Num() <= 0)
		{
			InitialisePolicies();
		}

		for (const FFoundationPoint& Point : Points)
		{
			const FVector Offset = FVector(0.f, 0.f, 1000.f);
			const FVector TraceStart = Point.Location + Offset;
			const FVector TraceEnd = Point.Location - Offset;

			Requests.Add
			(
				World->AsyncLineTraceByChannel(EAsyncTraceType::Single, TraceStart, TraceEnd, GridSettings->GridProjectionChannel, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam, &ValidationDelegate),
				Point
			);
		}

		Points.Empty();
	}

	void Stop()
	{
		ValidationDelegate.Unbind();

		for (TPair<FTraceHandle, FFoundationPoint>& Request : Requests)
		{
			Request.Key._Data.FrameNumber = 0;
		}

		Requests.Empty();

		Callback = nullptr;
	}

	void OnTraceComplete(const FTraceHandle& Handle, FTraceDatum& Data)
	{
		if (Requests.Num() <= 0)
		{
			return;
		}

		++NumCompletedTraces;

		if (Requests.Contains(Handle) && Data.OutHits.Num() > 0)
		{
			const FHitResult CurrHit = Data.OutHits[0];
			Requests[Handle].HitRes = CurrHit;
			Requests[Handle].Location = CurrHit.Location;

			RunPoliciesOnPoint(Requests[Handle]);
			Points.Add(Requests[Handle]);
		}

		if (Requests.Num() == Points.Num())
		{
			Callback(Points);
		}
	}

	void InitialisePolicies()
	{
		Policies.Empty();
		Policies.Reserve(2);

		Policies.Emplace(MakeShareable(new FTooSteepPointPolicy()));
		Policies.Emplace(MakeShareable(new FPointOccludedPointPolicy(*World)));
		//Too high policy
		//overlapping policy
	}

	void RunPoliciesOnPoint(FFoundationPoint& Point)
	{
		for (const TSharedPtr<FPointValidationPolicy> Policy : Policies)
		{
			if (!Policy->Run(Point))
			{
				Point.PolicyFailReasons.AddUnique(Policy->GetFailReason());
			}
		}
	}

private:

	UPROPERTY()
	TArray<FFoundationPoint> Points;

	TMap<FTraceHandle, FFoundationPoint> Requests;

	UPROPERTY()
	UWorld* World;

	TFunction<void(const TArray<FFoundationPoint>&)> Callback;

	UPROPERTY()
	const UWorldGridSettings* GridSettings;

	FTraceDelegate ValidationDelegate;
	uint8 NumCompletedTraces;

	TArray<TSharedPtr<FPointValidationPolicy>> Policies;
};
