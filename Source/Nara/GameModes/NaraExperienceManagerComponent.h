// Copyright NanoShiki. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LoadingProcessInterface.h"
#include "NaraExperienceManagerComponent.generated.h"

namespace UE::GameFeatures
{
	struct FResult;
}

class UNaraExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNaraExperienceLoaded, const UNaraExperienceDefinition* /*Experience*/);

enum class ENaraExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

/**
 * 
 */
UCLASS()
class NARA_API UNaraExperienceManagerComponent final : public UGameStateComponent, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	UNaraExperienceManagerComponent(const FObjectInitializer& ObjectInitializer);

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	//~ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~End of ILoadingProcessInterface interface

	//Experience加载
	void SetCurrentExperience(FPrimaryAssetId ExperienceId);

	//Experience加载完成后广播的委托(有优先级之分)
	void CallOrRegister_OnExperienceLoaded_HighPriority(FOnNaraExperienceLoaded::FDelegate&& Delegate);
	void CallOrRegister_OnExperienceLoaded(FOnNaraExperienceLoaded::FDelegate&& Delegate);
	void CallOrRegister_OnExperienceLoaded_LowPriority(FOnNaraExperienceLoaded::FDelegate&& Delegate);

	//如果当前的Experience已经被完全加载则返回. 
	const UNaraExperienceDefinition* GetCurrentExperienceChecked() const;

	//判断当前Experience是否被完全加载.
	bool IsExperienceLoaded() const;
private:
	UFUNCTION()
	void OnRep_CurrentExperience();

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();

private:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const UNaraExperienceDefinition> CurrentExperience;
	
	TArray<FString> GameFeaturePluginURLs;
	int32 NumGameFeaturePluginsLoading = 0;

	ENaraExperienceLoadState LoadState = ENaraExperienceLoadState::Unloaded;

	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	/** 用于做准备工作, 比如准备数据, 子系统启动. */
	FOnNaraExperienceLoaded OnExperienceLoaded_HighPriority;

	/** 用于标准的游戏逻辑初始化, 比如角色创建, UI加载. */
	FOnNaraExperienceLoaded OnExperienceLoaded;

	/** 用于收尾工作或依赖于其他系统就绪的逻辑. */
	FOnNaraExperienceLoaded OnExperienceLoaded_LowPriority;
};


