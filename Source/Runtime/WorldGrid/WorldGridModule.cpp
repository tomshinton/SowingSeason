#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FWorldGridModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FWorldGridModule, WorldGrid)