#pragma once

#include "ConstructionState.generated.h"

UENUM()
enum class EConstructionState : uint8 
{
	Framing, //Construction of timber frames/scaffolding
	Building, //Actual building
	Finished, //We can go ahead and spawn the building properly now
	Max	UMETA(Hidden)
};

USTRUCT()
struct FConstructionStateMap
{
	GENERATED_BODY()

public:

	FConstructionStateMap()
		: State()
		, Time(0.f)
	{};

	UPROPERTY(EditDefaultsOnly)
	EConstructionState State;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0, UIMin = 0))
	float Time;
};