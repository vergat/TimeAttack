// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimeAttackPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TIMEATTACK_API ATimeAttackPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	int32 currentLap;
	int32 maxLaps;
	float currentRaceTime;
	float currentLapTime;
	float goldTime;
	float silverTime;
	float bronzeTime;
	float bestRaceTime;
	float bestLapTime;
	float defaultBestRaceTime;
	float defaultBestLapTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText MapName;

	class UTimelineComponent* raceTimeline;
	class UTimelineComponent* lapTimeline;

	bool raceComplete;
	bool raceStart;

	FString saveSlot;
	//ATimeAttackSaveGame saveGameReference;

	FTransform respawnLocation;
	ATimeAttackPawn* vehicle;
	//UUserWidget* mywidget;

	FTimerHandle delay;
	FTimerDelegate timerDel;

	bool doOnceGuard;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText CurrentLapText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText MaxLapsText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText CurrentRaceTimeText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText CurrentLapTimeText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText GoldTimeText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText SilverTimeText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText BronzeTimeText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText BestRaceTimeText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText BestLapTimeText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText CountdownText;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY()
	class UUserWidget* currentWidget;

public:
	void RespawnVehicle();
	void UpdateLap();
	void EndLapUpdate();
	void EndRaceUpdate();
	void Restat();
	void Setup();


	UFUNCTION()
	void ResetBestTime();

	UFUNCTION()
		void StartRaceSequence();

	UPROPERTY(EditAnywhere, Category="Timeline")
	class UCurveFloat* timeCurve;

	FOnTimelineFloat RaceFunction{};
	FOnTimelineFloat LapFunction{};

	UFUNCTION()
		void RaceTimelineFloatReturn(float value);
	
	UFUNCTION()
		void LapTimelineFloatReturn(float value);
protected:
	UFUNCTION()
		void CountdownSequence(int32 value);
	UFUNCTION()
		void EndSequence();
	UFUNCTION()
		void RestartLevel();

private:
	void RefreshHUD();
	void SaveGameCheck();
	void SaveGame();
	void LoadGame();
};
