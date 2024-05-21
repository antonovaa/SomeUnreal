#include "Sound/SoundManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/Data/SoundRow.h"


template<class MessageType>
FString GetMessageToLog(const TCHAR* Condition, const MessageType& UserMessage)
{
	return FString::Format("Failed condition {}. {}", Condition, UserMessage);
}

#define CHECK_C(Condition, UserMessage)                                                \
{                                                                                              \
const FString MessageToLog = GetMessageToLog(TEXT(#Condition), UserMessage); \
UE_LOG(LogTemp, Error, "{}", MessageToLog);                                           \
}                                                                                          \
return false;                                                                              \
}())



USoundManagerSubsystem* USoundManagerSubsystem::GetSoundManager(const UObject* InWCO)
{
	//CHECK_C(InWCO, nullptr);
	//CHECK_C(InWCO->GetWorld(), nullptr);
	auto ResultSoundManager = InWCO->GetWorld()->GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
	//CHECK_C(ResultSoundManager, nullptr);
	return ResultSoundManager;
}

void USoundManagerSubsystem::StartPlay(const UObject* InWCO, const FName& InTag, ESoundLayerType InSoundLayer)
{
	//CHECK_C(SoundsDataTable, TEXT("SoundDataTable should exist!"));
	//CHECK_C(InWCO, TEXT("InWCO should be valid!"));
	//CHECK_C(!InTag.IsNone(), TEXT("InTag shouldn't be None!"));
	//CHECK_C(InSoundLayer != ESoundLayerType::None, TEXT("InSoundLayer shouldn't be None!"));

	FSoundRow* CurrentSoundRow = SoundsDataTable->FindRow<FSoundRow>(InTag, "");
	//CHECK_C(CurrentSoundRow, TEXT("Can't find sound by InTag!"));

	UAudioComponent* CurrentAudioComponent = GetSoundLayer(InSoundLayer, InWCO, CurrentSoundRow->Sound);
	//CHECK_C(CurrentAudioComponent, TEXT("Can't find or create AudioComponent!"));
	if (!CurrentAudioComponent->IsPlaying())
	{
		CurrentAudioComponent->Play();
	}
}

void USoundManagerSubsystem::StopPlay(ESoundLayerType InSoundLayer)
{
	//CHECK_C(InSoundLayer != ESoundLayerType::None, TEXT("InSoundLayer shouldn't be None!"));
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
		// don't create new background music if it already exists
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
