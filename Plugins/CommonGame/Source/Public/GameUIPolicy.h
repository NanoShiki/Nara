// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"

#include "GameUIPolicy.generated.h"

#define UE_API COMMONGAME_API

class UCommonLocalPlayer;
class UGameUIManagerSubsystem;
class ULocalPlayer;
class UPrimaryGameLayout;

/**
 * 本地多人游戏交互模式
 */
UENUM()
enum class ELocalMultiplayerInteractionMode : uint8
{
	// 仅为主玩家显示全屏视口，忽略其他玩家的存在
	PrimaryOnly,

	// 为一个玩家显示全屏视口，但玩家可以交换控制权，决定谁显示谁处于休眠状态
	SingleToggle,

	// 同时为所有玩家显示视口
	Simultaneous
};

/**
 * 根视口布局信息，用于跟踪本地玩家及其对应的UI布局在视口中的状态
 */
USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	/** 本地玩家 */
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	/** 该玩家的根UI布局 */
	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

	/** 布局是否已添加到视口 */
	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};

/**
 * 游戏UI策略类，负责管理游戏中UI的整体布局和行为
 * 定义了如何为玩家创建、管理和显示UI布局
 */
UCLASS(MinimalAPI, Abstract, Blueprintable, Within = GameUIManagerSubsystem)
class UGameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	template <typename GameUIPolicyClass = UGameUIPolicy>
	static GameUIPolicyClass* GetGameUIPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<GameUIPolicyClass>(GetGameUIPolicy(WorldContextObject));
	}

	static UE_API UGameUIPolicy* GetGameUIPolicy(const UObject* WorldContextObject);

public:
	UE_API virtual UWorld* GetWorld() const override;
	UE_API UGameUIManagerSubsystem* GetOwningUIManager() const;
	UE_API UPrimaryGameLayout* GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const;

	ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const { return LocalMultiplayerInteractionMode; }

	UE_API void RequestPrimaryControl(UPrimaryGameLayout* Layout);

protected:
	UE_API void AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	UE_API void RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	UE_API virtual void OnRootLayoutAddedToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	UE_API virtual void OnRootLayoutRemovedFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	UE_API virtual void OnRootLayoutReleased(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	UE_API void CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer);
	UE_API TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer);

private:
	/** 本地多人游戏交互模式 */
	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

	/** 布局类，用于创建玩家的根UI布局 */
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

	/** 根视口布局信息数组，跟踪所有本地玩家的UI布局状态 */
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

private:
	UE_API void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	UE_API void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	UE_API void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	friend class UGameUIManagerSubsystem;
};

#undef UE_API
