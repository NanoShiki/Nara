// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/AssetManager.h"
#include "NaraAssetManager.generated.h"

#define UE_API NARA_API

class UPrimaryDataAsset;

class ULyraGameData;
class ULyraPawnData;

struct FLyraBundles
{
	static const FName Equipped;
};


/**
 * UNaraAssetManager
 *
 *	Game implementation of the asset manager that overrides functionality and stores game-specific types.
 *	It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic.
 *	This class is used by setting 'AssetManagerClassName' in DefaultEngine.ini.
 */
UCLASS(MinimalAPI, Config = Game)
class UNaraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	UE_API UNaraAssetManager();

	// Returns the AssetManager singleton object.
	static UE_API UNaraAssetManager& Get();

};
#undef UE_API
