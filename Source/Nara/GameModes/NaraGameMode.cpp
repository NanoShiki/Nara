// Copyright NanoShiki. 保留所有权利。

#include "NaraGameMode.h"

#include "NaraExperienceDefinition.h"
#include "NaraExperienceManagerComponent.h"
#include "NaraGameState.h"
#include "Kismet/GameplayStatics.h"
#include "NaraWorldSettings.h"
#include "Nara/NaraLogChannels.h"
#include "Nara/System/NaraAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NaraGameMode)

ANaraGameMode::ANaraGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ANaraGameState::StaticClass();
}


void ANaraGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 等待下一帧，以便有时间初始化启动设置
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ANaraGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;	// 将要加载的Experience的ExperienceId
	FString ExperienceIdSource;	// 从哪个途径拿到的ExperienceId
	
	// 优先级顺序（最高优先）
	//  - 匹配分配（如果存在）
	//  - URL选项覆盖
	//  - 开发者设置（仅PIE）
	//  - 命令行覆盖
	//  - 世界设置
	//  - 专用服务器
	//  - 默认体验

	UWorld* World = GetWorld();

	// 下面这个OptionsString即一路传过来的URL							↓
	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		// 从OptionsString中拿到对应的Experience蓝图的名字. 比如B_LyraFrontEnd_Experience
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		// 利用拿到的名字查找对应的Asset
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UNaraExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
		ExperienceIdSource = TEXT("OptionsString");
	}

	// 检查命令行是否要设置体验
	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			if (!ExperienceId.PrimaryAssetType.IsValid())
			{
				ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UNaraExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromCommandLine));
			}
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	// 检查世界设置是否有默认体验
	if (!ExperienceId.IsValid())
	{
		if (ANaraWorldSettings* TypedWorldSettings = Cast<ANaraWorldSettings>(GetWorldSettings()))
		{
			ExperienceId = TypedWorldSettings->GetDefaultGameplayExperience();
			ExperienceIdSource = TEXT("WorldSettings");
		}
	}

	UNaraAssetManager& AssetManager = UNaraAssetManager::Get();
	FAssetData Dummy;
	if (ExperienceId.IsValid() && !AssetManager.GetPrimaryAssetData(ExperienceId, /*out*/ Dummy))
	{
		UE_LOG(LogNaraExperience, Error, TEXT("EXPERIENCE: 想要使用 %s 但找不到，将回退到默认值)"), *ExperienceId.ToString());
		ExperienceId = FPrimaryAssetId();
	}

	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void ANaraGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
	if (ExperienceId.IsValid())
	{
		UE_LOG(LogNaraExperience, Log, TEXT("已识别体验 %s (来源: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

		UNaraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UNaraExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->SetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogNaraExperience, Error, TEXT("无法识别体验，加载画面将永远保持"));
	}
}

void ANaraGameMode::OnExperienceLoaded(const UNaraExperienceDefinition* CurrentExperience)
{
	// 生成所有已连接的玩家
	//@TODO: 这里我们只处理 *玩家* 控制器，但在 GetDefaultPawnClassForController_Implementation 中我们跳过了所有控制器
	// 不过 GetDefaultPawnClassForController_Implementation 可能也只会为玩家调用
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool ANaraGameMode::IsExperienceLoaded() const
{
	check(GameState);
	UNaraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UNaraExperienceManagerComponent>();
	check(ExperienceComponent);

	return ExperienceComponent->IsExperienceLoaded();
}
