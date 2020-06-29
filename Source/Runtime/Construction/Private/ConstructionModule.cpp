#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FConstructionModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FConstructionModule, Construction)