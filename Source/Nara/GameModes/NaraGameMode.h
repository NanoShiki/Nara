// Copyright NanoShiki. 保留所有权利。

#pragma once

#include "ModularGameMode.h"

#include "NaraGameMode.generated.h"

#define UE_API NARA_API

class UNaraExperienceDefinition;

/**
 * ANaraGameMode
 *
 *	本项目使用的基础游戏模式类。
 */
UCLASS(MinimalAPI, Config = Game, Meta = (ShortTooltip = "本项目使用的基础游戏模式类。"))
class ANaraGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:

	UE_API ANaraGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBase 接口
	UE_API virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	UE_API virtual void InitGameState() override;
	UE_API virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	//~AGameModeBase 接口结束

protected:	
	UE_API void OnExperienceLoaded(const UNaraExperienceDefinition* CurrentExperience);
	UE_API bool IsExperienceLoaded() const;

	UE_API void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);

	UE_API void HandleMatchAssignmentIfNotExpectingOne();
};

#undef UE_API
