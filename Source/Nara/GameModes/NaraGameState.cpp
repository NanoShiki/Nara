// Copyright NanoShiki. 保留所有权利。

#include "NaraGameState.h"

#include "NaraExperienceManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NaraGameState)


ANaraGameState::ANaraGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UNaraExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}