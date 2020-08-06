#include <Runtime/Core/Public/Modules/ModuleManager.h>
 
class FAIBehaviourModule : public IModuleInterface
{
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FAIBehaviourModule, AIBehaviour)