#pragma once

#include <Runtime/CoreUObject/Public/UObject/GCObject.h>
#include <Runtime/Engine/Public/WorldCollision.h>
#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

DEFINE_LOG_CATEGORY_STATIC(PointValidationTaskLog, Log, Log)

class FPointValidator : public FGCObject
{
public:

	FPointValidator(const TArray<FFoundationPoint>& InPoints, UWorld& InWorld, const TFunction<void(const TArray<FFoundationPoint>&)>& InCallback)
		: Points(InPoints)
		, World(&InWorld)
		, Callback(InCallback)
		, GridSettings(GetDefault<UWorldGridSettings>())
		, ValidationDelegate()
		, NumCompletedTraces(0)
	{
		ValidationDelegate.BindRaw(this, &FPointValidator::OnTraceComplete);
	};

	void Run()
	{
		Requests.Reserve(Points.Num());

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
		for (TPair<FTraceHandle, FFoundationPoint>& Request : Requests)
		{
			Request.Key._Data.FrameNumber = 0;
		}
	}

	void OnTraceComplete(const FTraceHandle& Handle, FTraceDatum& Data)
	{
		++NumCompletedTraces;

		if (Requests.Contains(Handle) && Data.OutHits.Num() > 0)
		{
			const FHitResult CurrHit = Data.OutHits[0];
			Requests[Handle].HitRes = CurrHit;
			Requests[Handle].Location = CurrHit.Location;

			Points.Add(Requests[Handle]);
			Requests.Remove(Handle);
		}

		if (Requests.Num() == 0)
		{
			Callback(Points);
		}
	}

	void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		Collector.AddReferencedObject(GridSettings);
	}

private:

	UPROPERTY()
	TArray<FFoundationPoint> Points;

	UPROPERTY()
	TMap<FTraceHandle, FFoundationPoint> Requests;
	
	UPROPERTY()
	UWorld* World;

	TFunction<void(const TArray<FFoundationPoint>&)> Callback;

	UPROPERTY()
	const UWorldGridSettings* GridSettings;

	FTraceDelegate ValidationDelegate;
	uint8 NumCompletedTraces;
};