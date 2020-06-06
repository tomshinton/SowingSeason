#include "Runtime/Build/Public/Ghost/GhostRenderer.h"

AGhostRenderer::AGhostRenderer(const FObjectInitializer& ObjectInitializer)
	: SourceBuildingData(nullptr)
{

}

void AGhostRenderer::SetGhostInfo(const UBuildingData& InSourceBuildingData)
{
	SourceBuildingData = &InSourceBuildingData;
}