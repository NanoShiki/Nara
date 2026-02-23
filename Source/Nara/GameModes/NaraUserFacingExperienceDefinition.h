// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "NaraUserFacingExperienceDefinition.generated.h"

class FString;
class UCommonSession_HostSessionRequest;
class UObject;
class UTexture2D;
class UUserWidget;
struct FFrame;

/** 用于在UI中展示体验并启动新会话的设置描述 */
UCLASS(BlueprintType)
class UNaraUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** 要加载的具体地图 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="Map"))
	FPrimaryAssetId MapID;

	/** 要加载的游戏体验 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="NaraExperienceDefinition"))
	FPrimaryAssetId ExperienceID;

	/** 作为URL选项传递给游戏的额外参数 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience)
	TMap<FString, FString> ExtraArgs;

	/** UI中的主标题 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience)
	FText TileTitle;

	/** 副标题 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience)
	FText TileSubTitle;

	/** 完整描述 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience)
	FText TileDescription;

	/** UI中使用的图标 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience)
	TObjectPtr<UTexture2D> TileIcon;

	/** 加载到（或退出）指定体验时显示的加载界面控件 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=LoadingScreen)
	TSoftClassPtr<UUserWidget> LoadingScreenWidget;

	/** 如果为true，这是一个默认体验，应用于快速游戏并在UI中给予优先级 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience)
	bool bIsDefaultExperience = false;

	/** 如果为true，这将在前端的体验列表中显示 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience)
	bool bShowInFrontEnd = true;

	/** 此会话的最大玩家数量 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience)
	int32 MaxPlayerCount = 16;

public:
	/** 创建一个请求对象，用于使用这些设置实际启动会话 */
	/** meta = (WorldContext = "WorldContextObject")的作用是在蓝图事件图表中隐藏WorldContextObject这个输入引脚, 调用时会自动获取当前蓝图对象所在的World */
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta = (WorldContext = "WorldContextObject"))
	UCommonSession_HostSessionRequest* CreateHostingRequest(const UObject* WorldContextObject) const;
};
