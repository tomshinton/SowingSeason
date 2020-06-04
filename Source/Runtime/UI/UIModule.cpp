#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FUIModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FUIModule, UI)