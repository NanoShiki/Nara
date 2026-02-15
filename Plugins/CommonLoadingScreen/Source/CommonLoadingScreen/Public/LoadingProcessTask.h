#pragma once

#include "LoadingProcessInterface.h"

#include "LoadingProcessTask.generated.h"

#define UE_API COMMONLOADINGSCREEN_API

UCLASS(MinimalAPI, BlueprintType)
class ULoadingProcessTask : public UObject, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UE_API ULoadingProcessTask* CreateLoadingScreenProcessTask(UObject* WorldContextObject, const FString& ShowLoadingScreenReason);

public:
	ULoadingProcessTask() { }

	UFUNCTION(BlueprintCallable)
	UE_API void Unregister();

	UFUNCTION(BlueprintCallable)
	UE_API void SetShowLoadingScreenReason(const FString& InReason);

	UE_API virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;

	FString Reason;
};

#undef UE_API