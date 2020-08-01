#include "Runtime/Inventory/Public/Containers/InventoryItem.h"
#include "Runtime/Inventory/Public/InventorySettings.h"

void FInventoryItem::TryAdd(uint8& OutNum, const FName& InName, const FItemDataRow& InData)
{
	if (!Data.IsSet())
	{
		Name = InName;
		Data = InData;
	}

	Amount += OutNum;

	const uint8 StackSize = Data.GetValue().StackSize;
	if (Amount > StackSize)
	{
		OutNum = Amount - StackSize;
		Amount = StackSize;
	}
	else
	{
		OutNum = 0;
	}
}

void FInventoryItem::TryRemove(uint8& OutNum)
{
	//As we're using an overflow method on an unsigned type, temporarily use a signed type and reassign
	const int8 NewAmount = Amount - OutNum;

	if (NewAmount <= 0)
	{
		OutNum = FMath::Abs(NewAmount);
		Clear();
	}
	else
	{
		Amount = NewAmount;
		OutNum = 0;
	}
}

void FInventoryItem::GetData()
{
	Data = UInventorySettings::GetItemInfo(Name);
}

void FInventoryItem::Clear()
{
	Name = "";
	Amount = 0;
	Data = TOptional<FItemDataRow>();
}

bool FInventoryItem::CanAcceptItem(const FName& InItem) const
{
	return IsEmpty() || (!IsFull() && IsSameItem(InItem));
}

bool FInventoryItem::CanRemoveItem(const FName& InItem) const
{
	return !IsEmpty() && IsSameItem(InItem);
}

bool FInventoryItem::IsEmpty() const
{
	return !Data.IsSet();
}

bool FInventoryItem::IsFull() const
{
	if (Data.IsSet())
	{
		return Amount == Data.GetValue().StackSize;
	}

	return false;
}

bool FInventoryItem::IsSameItem(const FName& InName) const
{
	return Name == InName;
}
