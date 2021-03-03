// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "wurenjiGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WURENJI_API AwurenjiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	/*
	 * 全局变量：分数
	 */
	UPROPERTY()
	uint32 points;
};
