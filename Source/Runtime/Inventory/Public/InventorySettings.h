#pragma once

#include <Runtime/Engine/Classes/Engine/DeveloperSettings.h>

#include "Runtime/Inventory/Public/Containers/ItemDataRow.h"

#include "InventorySettings.generated.h"

DEFINE_LOG_CATEGORY_STATIC(InventorySettingsLog, Log, Log);

UCLASS(config = Game, defaultconfig, MinimalAPI)
class UInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UInventorySettings()
		: ItemLookupTable()
	{};

	UPROPERTY(config, EditDefaultsOnly, meta = (AllowedClasses = "DataTable"))
	FSoftObjectPath ItemLookupTable;

	static FItemDataRow GetItemInfo(const FName& InName)
	{
		if (!InName.IsValid())
		{
			UE_LOG(InventorySettingsLog, Warning, TEXT("No valid string provided in UInventorySettings::GetItemInfo - returning"))
			return FItemDataRow();
		}

		if (const UInventorySettings* Settings = GetDefault<UInventorySettings>())
		{
			if (UDataTable* Lookup = Cast<UDataTable>(Settings->ItemLookupTable.TryLoad()))
			{
				FString Context;
				if (FItemDataRow* FoundRow = Lookup->FindRow<FItemDataRow>(InName, Context))
				{
					return *FoundRow;
				}
				else
				{
					UE_LOG(InventorySettingsLog, Warning, TEXT("Could not find Item row for %s"), *InName.ToString());
				}
			}
		}

		return FItemDataRow();
	}
};