#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FInventoryModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FInventoryModule, Inventory)