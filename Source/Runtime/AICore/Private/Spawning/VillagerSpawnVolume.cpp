#include "Runtime/AICore/Public/Spawning/VillagerSpawnVolume.h"

#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include <Runtime/Engine/Classes/Components/SceneComponent.h>

namespace VillagerSpawnVolumePrivate
{
	const FName RootComponentName = TEXT("RootComponent");
	const FName VolumeName = TEXT("SpawnVolume");

#if WITH_EDITOR
	const FName SpawnerFolderPath = TEXT("Spawners");
#endif //WITH_EDITOR
}

AVillagerSpawnVolume::AVillagerSpawnVolume(const FObjectInitializer& ObjectInitialiser)
	: SpawnerRootComponent(ObjectInitialiser.CreateDefaultSubobject<USceneComponent>(this, VillagerSpawnVolumePrivate::RootComponentName))
	, Volume(ObjectInitialiser.CreateDefaultSubobject<UBoxComponent>(this, VillagerSpawnVolumePrivate::VolumeName))
{
	SetRootComponent(SpawnerRootComponent);
	Volume->SetupAttachment(RootComponent);

#if WITH_EDITOR
	if(!IsTemplate() && GetWorld())
	{
		SetFolderPath(VillagerSpawnVolumePrivate::SpawnerFolderPath);
	}
#endif //WITH_EDITOR
}
