#include "Runtime/UI/Public/Elements/Inventory/UI_InventoryItem.h"

#include <Runtime/UMG/Public/Components/HorizontalBox.h>
#include <Runtime/UMG/Public/Components/TextBlock.h>

UUI_InventoryItem::UUI_InventoryItem(const FObjectInitializer& ObjectInitialiser)
	: Super(ObjectInitialiser)
	, ItemBox(nullptr)
	, ItemName(nullptr)
	, Quantity(nullptr)
	, LastItem()
{

}

void UUI_InventoryItem::SetItem(const FInventoryItem& InItem)
{
	LastItem = InItem;

	Update();
}

void UUI_InventoryItem::Update()
{
	if (ItemName != nullptr)
	{
		if (LastItem.GetData().IsSet())
		{
			ItemBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			const FItemDataRow AssociatedData = LastItem.GetData().GetValue();
			ItemName->SetText(AssociatedData.ItemName);
			Quantity->SetText(FText::FromString(FString::FromInt(LastItem.Amount)));
		}
		else
		{
			ItemBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

