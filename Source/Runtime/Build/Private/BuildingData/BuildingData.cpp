#include "Runtime/Build/Public/BuildingData/BuildingData.h"

UBuildingData::UBuildingData()
	: BuildMode(EBuildMode::Single)
	, BuildingClass("")
	, ShouldCancelBuildPostPlacement(true)
	, NameReadable(TEXT("NAME_READABLE"))
{

}