#pragma once
#include "Engine/DeveloperSettingsBackedByCVars.h"

#include "CommonGameSettings.generated.h"

class UGameUIPolicy;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Common Game"))
class UCommonGameSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UCommonGameSettings();

public:
	UPROPERTY(config, EditAnywhere, Category=UI)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};
