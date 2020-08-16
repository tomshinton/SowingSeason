#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FAIIdentityModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FAIIdentityModule, AIIdentity)