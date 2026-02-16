// Copyright NanoShiki. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NaraExperienceActionSet.generated.h"

class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class NARA_API UNaraExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UNaraExperienceActionSet() { }

public:
	//该Experience要启用的GameFeatures.
	UPROPERTY(EditAnywhere, Category=FeatureDependencies)
	TArray<FString> GameFeaturesToEnable;

	//当Experience被loaded/activated/deactivated/unloaded时, 关联的Actions.
	UPROPERTY(EditAnywhere, Instanced, Category=ActionsToPerform)
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
