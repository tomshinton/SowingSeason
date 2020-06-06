#pragma once

#include "BuildMode.generated.h"

UENUM()
enum class EBuildMode : uint8 
{
	FireAndForget, 
	Linear, 
	Grid
};