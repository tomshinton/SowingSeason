#pragma once

#include "Runtime/Inventory/Public/Containers/ItemDataRow.h"

#include "InventoryItem.generated.h"

USTRUCT()
struct INVENTORY_API FInventoryItem
{
	GENERATED_BODY()

public:

	FInventoryItem(const FName& InName, const uint8 InAmount, const FItemDataRow& InData)
		: Name(InName)
		, Amount(InAmount)
		, Data(InData)
	{};

	FInventoryItem()
		: Name()
		, Amount(0)
		, Data()
	{};

	UPROPERTY(VisibleAnywhere)
	FName Name;

	UPROPERTY(VisibleAnywhere)
	uint8 Amount;

	TOptional<FItemDataRow> Data;

	void TryAdd(uint8& OutNum, const FName& InName, const FItemDataRow& InData);
	void TryRemove(uint8& OutNum);

	TOptional<FItemDataRow> GetData() const;
	void Clear();

	bool CanAcceptItem(const FName& InItem) const;
	bool CanRemoveItem(const FName& InItem) const;

	bool IsEmpty() const;
	bool IsFull() const;
	bool IsSameItem(const FName& InName) const;
};