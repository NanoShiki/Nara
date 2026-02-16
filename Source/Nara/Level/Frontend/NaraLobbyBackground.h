// Copyright NanoShiki. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NaraLobbyBackground.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UNaraLobbyBackground : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> BackgroundLevel;
};
