#include "Runtime/Build/Public/Ghost/BuildingGhost.h"

ABuildingGhost::ABuildingGhost(const FObjectInitializer& ObjectInitializer)
	: SourceBuildingData(nullptr)
{

}

void ABuildingGhost::SetGhostInfo(const UBuildingData& InSourceBuildingData)
{
	SourceBuildingData = &InSourceBuildingData;
}

void ABuildingGhost::DestroyGhost()
{
	Destroy();
}
