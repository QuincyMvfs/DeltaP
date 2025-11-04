#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class  EMovementStates:uint8
{
	Idle,
	Walking,
	Sprinting,
	Crouching
	
};
