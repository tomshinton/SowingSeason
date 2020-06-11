#pragma once

#include "Runtime/Build/Public/PointBuilder/FootprintPointBuilder.h"

DEFINE_LOG_CATEGORY_STATIC(PointBuilderFunctionsLog, Log, Log)

namespace PointBuilderFunctions
{
	static UPointBuilder* GetBuilderForMode(const UBuildingData& InBuildData, UObject& InOwner)
	{
		switch (InBuildData.BuildMode)
		{
		case EBuildMode::FireAndForget:
			return NewObject<UPointBuilder>(&InOwner, UFootprintPointBuilder::StaticClass());
			break;
		case EBuildMode::Grid:
			UE_LOG(PointBuilderFunctionsLog, Warning, TEXT("No Point Builder hooked up for Grid buildings"));
			break;
		case EBuildMode::Linear:
			UE_LOG(PointBuilderFunctionsLog, Warning, TEXT("No Point Builder hooked up for Linear buildings"));
			break;
		}

		return nullptr;
	}
}