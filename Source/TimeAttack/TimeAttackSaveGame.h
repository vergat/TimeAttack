// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "TimeAttackSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TIMEATTACK_API UTimeAttackSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UTimeAttackSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float SavedBestLapTime;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float SavedBestRaceTime;
	
};
