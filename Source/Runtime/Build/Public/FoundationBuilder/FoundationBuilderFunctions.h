// Sowing Season - Tom Shinton 2020

#pragma once

#include "Runtime/Build/Public/FoundationBuilder/FootprintFoundationBuilder.h"
#include "Runtime/Build/Public/FoundationBuilder/LinearFoundationBuilder.h"

DEFINE_LOG_CATEGORY_STATIC(PointBuilderFunctionsLog, Log, Log)

namespace FoundationBuilderFunctions
{
	static UFoundationBuilder* GetBuilderForMode(const UBuildingData& InBuildData, UObject& InOwner)
	{
		switch (InBuildData.BuildMode)
		{
		case EBuildMode::Single:
			return NewObject<UFoundationBuilder>(&InOwner, UFootprintFoundationBuilder::StaticClass());
			break;
		case EBuildMode::Grid:
			UE_LOG(PointBuilderFunctionsLog, Warning, TEXT("No Point Builder hooked up for Grid buildings"));
			break;
		case EBuildMode::Linear:
			return NewObject<ULinearFoundationBuilder>(&InOwner, ULinearFoundationBuilder::StaticClass());
			UE_LOG(PointBuilderFunctionsLog, Warning, TEXT("No Point Builder hooked up for Linear buildings"));
			break;
		}

		return nullptr;
	}
}
