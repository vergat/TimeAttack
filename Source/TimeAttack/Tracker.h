// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Checkpoint.h"
#include "TimeAttackPlayerController.h"
#include "Tracker.generated.h"

UCLASS()
class TIMEATTACK_API ATracker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATracker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnCheckpointPassed(int32 checkpointIndex);

private:
	UPROPERTY(EditAnywhere)
		TArray<ACheckpoint*> checkPoints;

	int32 totalCheckpoints;
	
	UPROPERTY(EditAnywhere)
		int32 maxLaps;

	UPROPERTY(EditAnywhere)
		float goldTime;
	
	UPROPERTY(EditAnywhere)
		float silverTime;
	
	UPROPERTY(EditAnywhere)
		float bronzeTime;
	
	UPROPERTY(EditAnywhere)
		float defaultBestLap;
	
	UPROPERTY(EditAnywhere)
		float defaultBestTime;

	ATimeAttackPlayerController *playerController;

	void SetupPlayerController(ATimeAttackPlayerController*);
	void SetupCheckpoints();
	void LapCheck(int32);
	bool RaceCompleteCheck();
};
