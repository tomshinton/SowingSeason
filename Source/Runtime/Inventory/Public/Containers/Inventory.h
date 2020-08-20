// Sowing Season - Tom Shinton 2020

#pragma once

#include "Runtime/Inventory/Public/Containers/InventoryItem.h"

#include "Runtime/Inventory/Public/Containers/InventoryAccessType.h"

#include "Inventory.generated.h"

USTRUCT()
struct FInventory
{
	GENERATED_BODY()

public:

	FInventory() {};

	UPROPERTY(EditAnywhere)
	uint8 Size;

	UPROPERTY(EditAnywhere)
	TArray<FInventoryItem> StartingItems;

	UPROPERTY(VisibleAnywhere)
	TArray<FInventoryItem> Items;

	UPROPERTY(EditAnywhere)
	EInventoryAccessType AccessType;

	void Initialise();
};
