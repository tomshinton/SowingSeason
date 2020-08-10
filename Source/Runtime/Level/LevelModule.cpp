// Sowing Season - Tom Shinton 2020

#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FLevelModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FLevelModule, Level)
