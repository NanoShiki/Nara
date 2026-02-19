#include "NaraAssetManager.h"

#include "Nara/NaraLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NaraAssetManager)

const FName FLyraBundles::Equipped("Equipped");

UNaraAssetManager::UNaraAssetManager()
{
	//DefaultPawnData = nullptr;
}

UNaraAssetManager& UNaraAssetManager::Get()
{
	check(GEngine);

	if (UNaraAssetManager* Singleton = Cast<UNaraAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogNara, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to NaraAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UNaraAssetManager>();
}