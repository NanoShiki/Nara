// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"

#include "GameUIManagerSubsystem.generated.h"

#define UE_API COMMONGAME_API

class FSubsystemCollectionBase;
class UCommonLocalPlayer;
class UGameUIPolicy;
class UObject;

/**
 * 这个管理器旨在被你的游戏实际需要的任何内容替换，
 * 所以这个类是抽象的，以防止它被直接创建。
 * 
 * 如果你只需要基本功能，你将从在自己的游戏中子类化这个子系统开始。
 */
UCLASS(MinimalAPI, Abstract, config = Game)
class UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UGameUIManagerSubsystem() { }
	
	UE_API virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UE_API virtual void Deinitialize() override;
	UE_API virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

	UE_API virtual void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	UE_API virtual void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	UE_API virtual void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

protected:
	UE_API void SwitchToPolicy(UGameUIPolicy* InPolicy);

private:
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;
};

#undef UE_API
