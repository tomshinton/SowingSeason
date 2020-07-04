// Sowing Season - Tom Shinton 2020

#include "Runtime/Level/Public/SowingSeasonLevel.h"

#include "Runtime/Level/Public/Masking/LevelMaskComponent.h"

namespace SowingSeasonLevelPrivate
{
	const FName MaskComponentName = TEXT("MaskComponent");
}

ASowingSeasonLevel::ASowingSeasonLevel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MaskComponent(ObjectInitializer.CreateDefaultSubobject<ULevelMaskComponent>(this, SowingSeasonLevelPrivate::MaskComponentName))
{

}
