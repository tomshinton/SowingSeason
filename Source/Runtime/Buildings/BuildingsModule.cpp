#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FBuildingsModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FBuildingsModule, Buildings)