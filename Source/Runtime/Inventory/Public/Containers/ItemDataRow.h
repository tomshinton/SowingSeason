// Sowing Season - Tom Shinton 2020

#pragma once

#include "Runtime/Engine/Classes/Engine/DataTable.h"

#include "ItemDataRow.generated.h"

USTRUCT()
struct INVENTORY_API FItemDataRow : public FTableRowBase
{

	GENERATED_BODY()

public:

	FItemDataRow()
		: ItemIcon()
		, StackSize(0)
		, ItemName()
	{};

	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath ItemIcon;

	UPROPERTY(EditDefaultsOnly)
	uint8 StackSize;

	UPROPERTY(EditDefaultsOnly)
	FText ItemName;
};
