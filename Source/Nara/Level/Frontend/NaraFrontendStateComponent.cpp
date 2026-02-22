// Copyright Epic Games, Inc. All Rights Reserved.

#include "NaraFrontendStateComponent.h"

#include "CommonGameInstance.h"
#include "CommonSessionSubsystem.h"
#include "CommonUserSubsystem.h"
#include "ControlFlowManager.h"
#include "Nara/GameModes/NaraExperienceManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NativeGameplayTags.h"
#include "PrimaryGameLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NaraFrontendStateComponent)

namespace FrontendTags
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PLATFORM_TRAIT_SINGLEONLINEUSER, "Platform.Trait.SingleOnlineUser");
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
}

UNaraFrontendStateComponent::UNaraFrontendStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UNaraFrontendStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the experience load to complete
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	UNaraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UNaraExperienceManagerComponent>();
	check(ExperienceComponent);

	// This delegate is on a component with the same lifetime as this one, so no need to unhook it in 
	ExperienceComponent->CallOrRegister_OnExperienceLoaded_HighPriority(FOnNaraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void UNaraFrontendStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UNaraFrontendStateComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (bShouldShowLoadingScreen)
	{
		OutReason = TEXT("Frontend Flow Pending...");
		
		if (FrontEndFlow.IsValid())
		{
			const TOptional<FString> StepDebugName = FrontEndFlow->GetCurrentStepDebugName();
			if (StepDebugName.IsSet())
			{
				OutReason = StepDebugName.GetValue();
			}
		}
		
		return true;
	}

	return false;
}

void UNaraFrontendStateComponent::OnExperienceLoaded(const UNaraExperienceDefinition* Experience)
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("FrontendFlow"))
		.QueueStep(TEXT("Wait For User Initialization"), this, &ThisClass::FlowStep_WaitForUserInitialization)
		.QueueStep(TEXT("Try Show Press Start Screen"), this, &ThisClass::FlowStep_TryShowPressStartScreen)
		.QueueStep(TEXT("Try Join Requested Session"), this, &ThisClass::FlowStep_TryJoinRequestedSession)
		.QueueStep(TEXT("Try Show Main Screen"), this, &ThisClass::FlowStep_TryShowMainScreen);

	Flow.ExecuteFlow();

	FrontEndFlow = Flow.AsShared();
}

void UNaraFrontendStateComponent::FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow)
{
	// If this was a hard disconnect, explicitly destroy all user and session state
	// TODO: Refactor the engine disconnect flow so it is more explicit about why it happened
	bool bWasHardDisconnect = false;
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>();
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);

	if (ensure(GameMode) && UGameplayStatics::HasOption(GameMode->OptionsString, TEXT("closed")))
	{
		bWasHardDisconnect = true;
	}

	// Only reset users on hard disconnect
	UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();
	if (ensure(UserSubsystem) && bWasHardDisconnect)
	{
		UserSubsystem->ResetUserState();
	}

	// Always reset sessions
	UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>();
	if (ensure(SessionSubsystem))
	{
		SessionSubsystem->CleanUpSessions();
	}

	SubFlow->ContinueFlow();
}

void UNaraFrontendStateComponent::FlowStep_TryShowPressStartScreen(FControlFlowNodeRef SubFlow)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();

	//如果已经登陆过(离线登陆或者在线登陆), 就不需要再显示登陆按钮.
	//这样一来, 玩家在重新回到主菜单时就不会再次显示登陆按钮.
	if (const UCommonUserInfo* FirstUser = UserSubsystem->GetUserInfoForLocalPlayerIndex(0))
	{
		if (FirstUser->InitializationState == ECommonUserInitializationState::LoggedInLocalOnly ||
			FirstUser->InitializationState == ECommonUserInitializationState::LoggedInOnline)
		{
			SubFlow->ContinueFlow();
			return;
		}
	}

	//检查平台是否确实需要一个“按开始”屏幕。
	//这仅在有多个在线用户的平台上才需要，(一般为PlayStation等主机平台)
	//在这些平台上，根据玩家按“开始”键的控制器，确定玩家实际登录游戏所用的身份。
	if (!UserSubsystem->ShouldWaitForStartInput())
	{
		// Start the auto login process, this should finish quickly and will use the default input device id
		InProgressPressStartScreen = SubFlow;
		UserSubsystem->OnUserInitializeComplete.AddDynamic(this, &UNaraFrontendStateComponent::OnUserInitialized);
		UserSubsystem->TryToInitializeForLocalPlay(0, FInputDeviceId(), false);

		return;
	}

	//如果需要显示PressStartScreen, 那么由其EventGraph来调用TryToInitializeForLocalPlay函数.
	//在该UI Deactivate的时候, ContinueFlow.
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
		constexpr bool bSuspendInputUntilComplete = true;
		RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(FrontendTags::TAG_UI_LAYER_MENU, bSuspendInputUntilComplete, PressStartScreenClass,
			[this, SubFlow](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen) {
			switch (State)
			{
			case EAsyncWidgetLayerState::AfterPush:
				bShouldShowLoadingScreen = false;
				Screen->OnDeactivated().AddWeakLambda(this, [this, SubFlow]() {
					SubFlow->ContinueFlow();
				});
				break;
			case EAsyncWidgetLayerState::Canceled:
				bShouldShowLoadingScreen = false;
				SubFlow->ContinueFlow();
				return;
			}
		});
	}
}

void UNaraFrontendStateComponent::OnUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext)
{
	FControlFlowNodePtr FlowToContinue = InProgressPressStartScreen;
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();

	if (ensure(FlowToContinue.IsValid() && UserSubsystem))
	{
		UserSubsystem->OnUserInitializeComplete.RemoveDynamic(this, &UNaraFrontendStateComponent::OnUserInitialized);
		InProgressPressStartScreen.Reset();

		if (bSuccess)
		{
			// On success continue flow normally
			FlowToContinue->ContinueFlow();
		}
		else
		{
			// TODO: Just continue for now, could go to some sort of error screen
			FlowToContinue->ContinueFlow();
		}
	}
}

void UNaraFrontendStateComponent::FlowStep_TryJoinRequestedSession(FControlFlowNodeRef SubFlow)
{
	UCommonGameInstance* GameInstance = Cast<UCommonGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance->GetRequestedSession() != nullptr && GameInstance->CanJoinRequestedSession())
	{
		UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>();
		if (ensure(SessionSubsystem))
		{
			// Bind to session join completion to continue or cancel the flow
			// TODO:  Need to ensure that after session join completes, the server travel completes.
			OnJoinSessionCompleteEventHandle = SessionSubsystem->OnJoinSessionCompleteEvent.AddWeakLambda(this, [this, SubFlow, SessionSubsystem](const FOnlineResultInformation& Result)
			{
				// Unbind delegate. SessionSubsystem is the object triggering this event, so it must still be valid.
				SessionSubsystem->OnJoinSessionCompleteEvent.Remove(OnJoinSessionCompleteEventHandle);
				OnJoinSessionCompleteEventHandle.Reset();

				if (Result.bWasSuccessful)
				{
					// No longer transitioning to the main menu
					SubFlow->CancelFlow();
				}
				else
				{
					// Proceed to the main menu
					SubFlow->ContinueFlow();
					return;
				}
			});
			GameInstance->JoinRequestedSession();
			return;
		}
	}
	// Skip this step if we didn't start requesting a session join
	SubFlow->ContinueFlow();
}

void UNaraFrontendStateComponent::FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow)
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
		constexpr bool bSuspendInputUntilComplete = true;
		RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(FrontendTags::TAG_UI_LAYER_MENU, bSuspendInputUntilComplete, MainScreenClass,
			[this, SubFlow](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen) {
			switch (State)
			{
			case EAsyncWidgetLayerState::AfterPush:
				bShouldShowLoadingScreen = false;
				SubFlow->ContinueFlow();
				return;
			case EAsyncWidgetLayerState::Canceled:
				bShouldShowLoadingScreen = false;
				SubFlow->ContinueFlow();
				return;
			}
		});
	}
}

