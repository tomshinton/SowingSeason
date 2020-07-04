#include "Runtime/Level/Public/Masking/LevelMaskComponent.h"

#include <Runtime/Engine/Public/EngineUtils.h>
#include <Runtime/Landscape/Classes/Landscape.h>
#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

DEFINE_LOG_CATEGORY_STATIC(LevelMaskComponentLog, Log, Log);

namespace LevelMaskComponentPrivate
{
	const FName MaskName = TEXT("LevelMask");
}

ULevelMaskComponent::ULevelMaskComponent()
	: ShouldGenerateMask(false)
	, Mask(nullptr)
	, WorldGridSettings(GetDefault<UWorldGridSettings>())	
	, SourceBounds(FVector::ZeroVector)
{

}

UTexture2D* ULevelMaskComponent::GetMask()
{
	return Mask;
}

void ULevelMaskComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ShouldGenerateMask)
	{
		UE_LOG(LevelMaskComponentLog, Log, TEXT("Looking for Level Landscape"));

		ALandscape* FoundLandscape = nullptr;

		for (TActorIterator<ALandscape> Itr(GetWorld()); Itr; ++Itr)
		{
			if (ALandscape* Landscape = *Itr)
			{
				FoundLandscape = Landscape;

				FVector Origin;
				FVector RawSource;
				Landscape->GetActorBounds(false, Origin, RawSource);

				SourceBounds = FVector2D(RawSource) * 2;

				UE_LOG(LevelMaskComponentLog, Log, TEXT("Found: %s, initialising Mask to match dimensions (%s)"), *Landscape->GetName(), *SourceBounds.ToString());
				break;
			}
		}

		if (FoundLandscape != nullptr)
		{
			GenerateMask();
		}
	}
}

void ULevelMaskComponent::GenerateMask()
{
	SanitizeBounds();

	if (SourceBounds.Size() > 0.f)
	{
		UE_LOG(LevelMaskComponentLog, Log, TEXT("Generating mask for sanitized bounds %s"), *SourceBounds.ToString());

		const int32 MaskX = SourceBounds.X / WorldGridSettings->GridCellSize;
		const int32 MaskY = SourceBounds.Y / WorldGridSettings->GridCellSize;

		UE_LOG(LevelMaskComponentLog, Log, TEXT("Bounds producing %ix%i Mask with 4 channels of data"), MaskX, MaskY);

		if (MaskX > 0 && MaskY > 0)
		{
			Mask = NewObject<UTexture2D>(GetTransientPackage(), LevelMaskComponentPrivate::MaskName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

			if(Mask == nullptr)
			{
				UE_LOG(LevelMaskComponentLog, Error, TEXT("Could not create initial Level Mask"));
				return;
			}

			Mask->PlatformData = new FTexturePlatformData();
			Mask->PlatformData->SizeX = MaskX;
			Mask->PlatformData->SizeY = MaskY;
			Mask->PlatformData->NumSlices = 1;
			Mask->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;

			Mask->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
			Mask->LODGroup = TextureGroup::TEXTUREGROUP_UI;

			/*EXPENSIVE BIT*/
			uint8* Pixels = new uint8[MaskX * MaskY * 4];
			for (int32 y = 0; y < MaskY; y++)
			{
				for (int32 x = 0; x < MaskX; x++)
				{
					const int32 CurrentPixelIndex = ((y * MaskX) + x);
					const FColor RandomColor = FColor::MakeRandomColor();

					Pixels[4 * CurrentPixelIndex] = RandomColor.B; //b
					Pixels[4 * CurrentPixelIndex + 1] = RandomColor.G; //g
					Pixels[4 * CurrentPixelIndex + 2] = RandomColor.R; //r
					Pixels[4 * CurrentPixelIndex + 3] = RandomColor.A; //set A channel always to maximum
				}
			}

			//Create the first mip - this is a pixel mask so we only need one
			FTexture2DMipMap* Mip = new FTexture2DMipMap();
			Mask->PlatformData->Mips.Add(Mip);
			Mip->SizeX = MaskX;
			Mip->SizeY = MaskY;

			//Lock the mipmap data so it can be modified
			Mip->BulkData.Lock(LOCK_READ_WRITE);
			uint8* TextureData = (uint8*)Mip->BulkData.Realloc(MaskX * MaskY * 4);

			//Copy the pixel data into the Texture data
			FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * MaskY * MaskX * 4);
			Mip->BulkData.Unlock();

			Mask->Source.Init(MaskX, MaskY, 1, 0, ETextureSourceFormat::TSF_BGRA8, Pixels);
			Mask->UpdateResource();

			//Address aliasing
			Mask->AddressX = TextureAddress::TA_Clamp;
			Mask->AddressY = TextureAddress::TA_Clamp;
			Mask->Filter = TextureFilter::TF_Nearest;

			delete[] Pixels;
		}
	}
}

void ULevelMaskComponent::SanitizeBounds()
{
	SourceBounds = FVector2D(FMath::FloorToFloat(SourceBounds.X), FMath::FloorToFloat(SourceBounds.Y));
}