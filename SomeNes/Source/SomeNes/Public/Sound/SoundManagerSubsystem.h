#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/AudioComponent.h"
#include "Sound/Data/SoundLayerData.h"
#include "Engine/DataTable.h"
#include "SoundManagerSubsystem.generated.h"

UCLASS()
class SOMENES_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	static USoundManagerSubsystem* GetSoundManager(const UObject* InWCO);

	UFUNCTION(BlueprintCallable, Category = "SoundManagerSubsystem", meta = (WorldContext = "InWCO"))
	void StartPlay(const UObject* InWCO, const FName& InTag, ESoundLayerType InSoundLayer);

	UFUNCTION(BlueprintCallable, Category = "SoundManagerSubsystem")
	void StopPlay(ESoundLayerType InSoundLayer);

private:
	UAudioComponent* GetSoundLayer(ESoundLayerType InSoundLayer, const UObject* InWCO = nullptr, USoundBase* InSound = nullptr);

	UPROPERTY()
	UAudioComponent* BackgroundMusic = nullptr;

	UPROPERTY()
	UDataTable* SoundsDataTable = nullptr;
};