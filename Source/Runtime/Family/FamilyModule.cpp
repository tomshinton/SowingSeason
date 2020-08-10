#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FFamilyModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FFamilyModule, Family)