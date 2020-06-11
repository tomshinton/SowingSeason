#include "Runtime/Build/Public/BuildingData/BuildingData.h"

UBuildingData::UBuildingData()
	: BuildMode(EBuildMode::FireAndForget)
	, BuildingClass("")
	, ShouldCancelBuildPostPlacement(true)
	, NameReadable(TEXT("NAME_READABLE"))
{

}