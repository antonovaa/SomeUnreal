#pragma once

#include "Engine/DataTable.h"
#include "SoundRow.generated.h"

USTRUCT(BlueprintType)
struct FSoundRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* Sound = nullptr;
};