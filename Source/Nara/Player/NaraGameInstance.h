// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonGameInstance.h"

#include "NaraGameInstance.generated.h"

#define UE_API NARA_API

class ANaraPlayerController;
class UObject;

UCLASS(MinimalAPI, Config = Game)
class UNaraGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

public:
	UE_API UNaraGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

#undef UE_API
