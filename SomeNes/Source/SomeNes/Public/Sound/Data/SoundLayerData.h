#pragma once

#include "CoreMinimal.h"
#include "SoundLayerData.generated.h"

UENUM(BlueprintType)
enum class ESoundLayerType : uint8
{
	None = 0,
	BackgroundMusic = 1,
};