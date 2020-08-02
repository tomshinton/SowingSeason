// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "Runtime/Inventory/Public/Containers/Inventory.h"
#include "Runtime/Inventory/Public/InventoryInterface.h"
#include "Runtime/Inventory/Public/InventorySettings.h"

#include "InventoryComponent.generated.h"

UCLASS()
class INVENTORY_API UInventoryComponent : public UActorComponent
	, public IInventoryInterface
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

private:
	
	void BeginPlay() override;

	//IInventoryInterface
	FOnInventoryChanged& GetOnInventoryChanged() override { return OnInventoryChanged; };
	void AddItem(const uint8 NumToAdd, const FName& InItem) override;
	void RemoveItem(const uint8 InNumToRemove, const FName& InItem) override;
	TArray<FInventoryItem> GetItems() const override;
	//~IInventoryInterface

	UPROPERTY()
	const UInventorySettings* InventorySettings;

	UPROPERTY(EditAnywhere)
	FInventory Inventory;

private:

	FOnInventoryChanged OnInventoryChanged;
};
