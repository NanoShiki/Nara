// Copyright NanoShiki. All Rights Reserved.

#include "NaraWorldSettings.h"

#include "Engine/AssetManager.h"
#include "Nara/NaraLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NaraWorldSettings)

ANaraWorldSettings::ANaraWorldSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FPrimaryAssetId ANaraWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId Result;
	if (!DefaultGameplayExperience.IsNull())
	{
		Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());

		if (!Result.IsValid())
		{
			UE_LOG(LogNaraExperience, Error, TEXT("%s.DefaultGameplayExperience 是 %s, 但是无法解析为资产ID (您可能需要在游戏功能插件或项目设置中添加资产规则)"),
				*GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
		}
	}
	
	return Result;
}
