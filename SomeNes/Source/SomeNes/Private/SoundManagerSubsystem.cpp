#include "Sound/SoundManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/Data/SoundRow.h"


template<class MessageType>
FString GetMessageToLog(const TCHAR* Condition, const MessageType& UserMessage)
{
	return FString::Format("Failed condition {}. {}", Condition, UserMessage);
}

#define CHECK_Con(Condition, UserMessage)                                                \
{                                                                                              \
const FString MessageToLog = GetMessageToLog(TEXT(#Condition), UserMessage); \
UE_LOG(LogTemp, Error, "{}", MessageToLog);                                           \
}                                                                                          \
return false;                                                                              \
}())



USoundManagerSubsystem* USoundManagerSubsystem::GetSoundManager(const UObject* InWCO)
{
	auto ResultSoundManager = InWCO->GetWorld()->GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
	return ResultSoundManager;
}

void USoundManagerSubsystem::StartPlay(const UObject* InWCO, const FName& InTag, ESoundLayerType InSoundLayer)
{
	FSoundRow* CurrentSoundRow = SoundsDataTable->FindRow<FSoundRow>(InTag, "");
	UAudioComponent* CurrentAudioComponent = GetSoundLayer(InSoundLayer, InWCO, CurrentSoundRow->Sound);
	if (!CurrentAudioComponent->IsPlaying())
	{
		CurrentAudioComponent->Play();
	}
}

void USoundManagerSubsystem::StopPlay(ESoundLayerType InSoundLayer)
{
	if (UAudioComponent* CurrentAudioComponent = GetSoundLayer(InSoundLayer))
	{
		CurrentAudioComponent->Stop();
	}
}

UAudioComponent* USoundManagerSubsystem::GetSoundLayer(ESoundLayerType InSoundLayer, const UObject* InWCO, USoundBase* InSound)
{
	const auto SetAudioComponent = [&](UAudioComponent*& InAudioComponent)
	{
		if (InAudioComponent && InSound)
		{
			InAudioComponent->SetSound(InSound);
		}
		else if (!InAudioComponent && InWCO && InSound)
		{
			InAudioComponent = UGameplayStatics::CreateSound2D(InWCO, InSound, 1, 1, 0, nullptr, true);
		}
	};

	switch (InSoundLayer)
	{
	case ESoundLayerType::BackgroundMusic:
		if (BackgroundMusic && BackgroundMusic->IsPlaying())
		{
			return BackgroundMusic;
		}

		SetAudioComponent(BackgroundMusic);

		return BackgroundMusic;
	default:
		break;
	}

	return nullptr;
}
