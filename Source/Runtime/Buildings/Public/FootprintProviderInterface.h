#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "FootprintProviderInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFootprintProvider : public UInterface
{
	GENERATED_BODY()
};

class IFootprintProvider
{
	GENERATED_BODY()

public:

	virtual const UPrimitiveComponent& GetFootprintReference() const = 0;
};
#pragma once
