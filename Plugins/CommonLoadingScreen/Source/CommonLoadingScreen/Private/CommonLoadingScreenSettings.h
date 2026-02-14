#pragma once
#include "Engine/DeveloperSettingsBackedByCVars.h"


#include "CommonLoadingScreenSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Common Loading Screen"))
class UCommonLoadingScreenSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UCommonLoadingScreenSettings();

public:
	// Loading界面要显示的Widgt
	UPROPERTY(Config, EditAnywhere, Category=Display, meta=(MetaClass="/Script/UMG.UserWidget"))
	FSoftClassPath LoadingScreenWidget;

	//确保Loading界面位于Viewport最顶层
	UPROPERTY(Config, EditAnywhere, Category=Display)
	int32 LoadingScreenZOrder = 10000;

	// 加载完毕后, 预留一段时间给texture streaming, 避免画面模糊.
	// 注意: 通常不会将其应用到编辑器中, 但可以通过HoldLoadingScreenAdditionalSecsEvenInEditor启用它.
	// 注意: 请在控制台中设置此变量, 在设置界面设置的值无效
	UPROPERTY(Config, EditAnywhere, Category=Configuration, meta=(ForceUnits=s, ConsoleVariable="CommonLoadingScreen.HoldLoadingScreenAdditionalSecs"))
	float HoldLoadingScreenAdditionalSecs = 2.0f;

	// 如果不为0, 并且加载时间超过这个变量时, 认为是卡死了.
	UPROPERTY(Config, EditAnywhere, Category=Configuration, meta=(ForceUnits=s))
	float LoadingScreenHeartbeatHangDuration = 0.0f;

	// 每间隔该时间打印一次加载界面显示的原因.
	UPROPERTY(Config, EditAnywhere, Category=Configuration, meta=(ForceUnits=s))
	float LogLoadingScreenHeartbeatInterval = 5.0f;

	// 如果为true, 每帧打印加载界面显示或隐藏的原因
	// 注意: 该变量被标记为Transient, 每次重启编辑器的时候会将其值置为0.
	// 注意: 请在控制台中设置此变量, 在设置界面设置的值无效
	UPROPERTY(Transient, EditAnywhere, Category=Debugging, meta=(ConsoleVariable="CommonLoadingScreen.LogLoadingScreenReasonEveryFrame"))
	bool LogLoadingScreenReasonEveryFrame = false;

	// 如果为true, 则强制显示Loading界面
	// 注意: 该变量被标记为Transient, 每次重启编辑器的时候会将其值置为0.
	// 注意: 请在控制台中设置此变量, 在设置界面设置的值无效
	UPROPERTY(Transient, EditAnywhere, Category=Debugging, meta=(ConsoleVariable="CommonLoadingScreen.AlwaysShow"))
	bool ForceLoadingScreenVisible = false;

	// 如果为true, 无论是否在编辑器中, 都预留HoldLoadingScreenAdditionalSecs时间.
	// 注意: 该变量被标记为Transient, 每次重启编辑器的时候会将其值置为0. 
	UPROPERTY(Transient, EditAnywhere, Category=Debugging)
	bool HoldLoadingScreenAdditionalSecsEvenInEditor = false;

	// 是否调用Slate的Tick函数, 确保Loading界面马上显示.
	UPROPERTY(config, EditAnywhere, Category=Configuration)
	bool ForceTickLoadingScreenEvenInEditor = true;
};
