#pragma once

#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "SelectionInterface.generated.h"

UINTERFACE(MinimalAPI)
class USelectionInterface : public UInterface
{
	GENERATED_BODY()
};

class ISelectionInterface
{
	GENERATED_BODY()

public:

	virtual void OnSelected() = 0;
	virtual void OnUnselected() = 0;

	virtual TSubclassOf<UUserWidget> GetSelectionBoxClass() const = 0;
	virtual FVector GetSelectionBoxLocation() const = 0;

#if !UE_BUILD_SHIPPING
	virtual FString GetDebugName() const = 0;
#endif //!UE_BUILD_SHIPPING
};
