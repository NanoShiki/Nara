#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"

#include "LoadingScreenManager.generated.h"

#define UE_API COMMONLOADINGSCREEN_API

template <typename InterfaceType> class TScriptInterface;

class ILoadingProcessInterface;

UCLASS(MinimalAPI)
class ULoadingScreenManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	//~USubsystem interface
	UE_API virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UE_API virtual void Deinitialize() override;
	UE_API virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End of USubsystem interface

	//~FTickableGameObject interface
	UE_API virtual void Tick(float DeltaTime) override;
	UE_API virtual ETickableTickType GetTickableTickType() const override;
	UE_API virtual bool IsTickable() const override;
	UE_API virtual TStatId GetStatId() const override;
	UE_API virtual UWorld* GetTickableGameObjectWorld() const override;
	//~End of FTickableGameObject interface

	UFUNCTION(BlueprintCallable, Category=LoadingScreen)
	FString GetDebugReasonForShowingOrHidingLoadingScreen() const
	{
		return DebugReasonForShowingOrHidingLoadingScreen;
	}

	/** 如果当前Loading界面被显示, 返回true */
	bool GetLoadingScreenDisplayStatus() const
	{
		return bCurrentlyShowingLoadingScreen;
	}

	/** 
	 * 当Loading界面显示状态发生改变时调用
	 * 主要用于音乐播放, 视效播放等
	 */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoadingScreenVisibilityChangedDelegate, bool);
	FORCEINLINE FOnLoadingScreenVisibilityChangedDelegate& OnLoadingScreenVisibilityChangedDelegate()
	{
		return LoadingScreenVisibilityChanged;
	}

	UE_API void RegisterLoadingProcessor(TScriptInterface<ILoadingProcessInterface> Interface);
	UE_API void UnregisterLoadingProcessor(TScriptInterface<ILoadingProcessInterface> Interface);

private:
	UE_API void HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName);
	UE_API void HandlePostLoadMap(UWorld* World);

	/** 判断当前是否显示Loading界面. 每帧调用 */
	UE_API void UpdateLoadingScreen();
	UE_API bool ShouldShowLoadingScreen();
	UE_API bool CheckForAnyNeedToShowLoadinGScreen();

	/** 显示 */
	UE_API void ShowLoadingScreen();
	UE_API bool IsShowingInitialLoadingScreen() const;
	UE_API void StartBlockingInput();
	UE_API void ChangePerformanceSettings(bool bEnableLoadingScreen);

	/** 回收 */
	UE_API void HideLoadingScreen();
	UE_API void StopBlockingInput();
	UE_API void RemoveWidgetFromViewport();
	
private:
	/** 当Loading界面显示状态发生改变时广播该委托 */
	FOnLoadingScreenVisibilityChangedDelegate LoadingScreenVisibilityChanged;

	/** 所有注册的LoadingProcessor */
	TArray<TWeakInterfacePtr<ILoadingProcessInterface>> ExternalLoadingProcessors;

	/** 要显示的Widget */
	TSharedPtr<SWidget> LoadingScreenWidget;

	/** 处理输入 */
	TSharedPtr<IInputProcessor> InputPreProcessor;
	
	/** 显示或隐藏Loading界面的原因 */
	FString DebugReasonForShowingOrHidingLoadingScreen;

	/** 当显示Loading界面时为true */
	bool bCurrentlyShowingLoadingScreen = false;

	/** 当我们处于PreLoadMap和PostLoadMap时, 该变量为true */
	bool bCurrentlyInLoadMap = false;

	/** 还有多久才再次打印Loading界面显示原因 */
	double TimeUntilNextLogHeartbeatSeconds = 0.0;

	/** Loading界面将要被隐藏的时间 */
	double TimeLoadingScreenLastDismissed = -1.0;

	/** 开始显示Loading界面的时间 */
	double TimeLoadingScreenShown = 0.0;
};

#undef UE_API
