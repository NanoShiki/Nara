// Copyright NanoShiki. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "NaraWorldSettings.generated.h"

class UNaraExperienceDefinition;
/**
 * 
 */
UCLASS()
class NARA_API ANaraWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	ANaraWorldSettings(const FObjectInitializer& ObjectInitializer);

	//如果没有使用UserFacingExperience加载该Level, 会用此函数尝试获取该Level的默认Experience. 
	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
    //该Level的默认Experience
    UPROPERTY(EditDefaultsOnly, Category=GameMode)
    TSoftClassPtr<UNaraExperienceDefinition> DefaultGameplayExperience;

public:

#if WITH_EDITORONLY_DATA
	//该Level是否是主菜单或其他standalone Experience?
	//如果为true, 那么当你点击Play in Editor时, NetMode回强制变成Standalone
	UPROPERTY(EditDefaultsOnly, Category=PIE)
	bool ForceStandaloneNetMode = false;
#endif
};
