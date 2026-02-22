// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonLocalPlayer.h"

#include "NaraLocalPlayer.generated.h"

#define UE_API NARA_API

/**
 * UNaraLocalPlayer
 */
UCLASS(MinimalAPI)
class UNaraLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()
};

#undef UE_API
