// Copyright NanoShiki. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NaraExperienceDefinition.generated.h"

class UNaraExperienceActionSet;
class UGameFeatureAction;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class NARA_API UNaraExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UNaraExperienceDefinition() { }
	
public:
	//该Experience要启用的GameFeatures.
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;

	//TODO: DefaultPawnData

	//当Experience被loaded/activated/deactivated/unloaded时, 关联的Actions.
	UPROPERTY(EditDefaultsOnly, Instanced, Category=Actions)
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	//要使用的ActionSets. 每个ActionSets封装了Actions和GameFeatures.
	UPROPERTY(EditDefaultsOnly, Instanced, Category=ActionSets)
	TArray<TObjectPtr<UNaraExperienceActionSet>> ActionSets;
};
