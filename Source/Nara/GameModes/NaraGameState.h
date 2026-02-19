// Copyright NanoShiki. 保留所有权利。

#pragma once

#include "ModularGameState.h"

#include "NaraGameState.generated.h"

#define UE_API NARA_API

class UNaraExperienceManagerComponent;
/**
 * ANaraGameState
 *
 *	本项目使用的基础游戏状态类。
 */
UCLASS(MinimalAPI, Config = Game)
class ANaraGameState : public AModularGameStateBase
{
	GENERATED_BODY()


	
public:

	UE_API ANaraGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
private:
	// 负责加载和管理当前的游戏体验
	UPROPERTY()
	TObjectPtr<UNaraExperienceManagerComponent> ExperienceManagerComponent;
};

#undef UE_API
