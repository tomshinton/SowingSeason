#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "InventoryInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryChanged);

struct FInventoryItem;

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class IInventoryInterface
{
	GENERATED_BODY()

public:

	virtual FOnInventoryChanged& GetOnInventoryChanged() = 0;
	virtual void AddItem(const uint8 NumToAdd, const FName& InItem) = 0;
	virtual void RemoveItem(const uint8 InNumToRemove, const FName& InItem) = 0;
	virtual TArray<FInventoryItem> GetItems() const = 0;
};
