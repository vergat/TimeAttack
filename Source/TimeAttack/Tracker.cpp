// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeAttack.h"
#include "Tracker.h"


// Sets default values
ATracker::ATracker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATracker::BeginPlay()
{
	Super::BeginPlay();
	playerController=Cast<ATimeAttackPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0));
	SetupPlayerController();
	SetupCheckpoints();
}

// Called every frame
void ATracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATracker::OnCheckpointPassed(int32 checkpointIndex)
{
	LapCheck(checkpointIndex);
	if (playerController->raceComplete)
	{
		playerController->Restat();
	}
}

void ATracker::SetupPlayerController()
{
	playerController->goldTime = goldTime;
	playerController->silverTime = silverTime;
	playerController->bronzeTime = bronzeTime;
	playerController->defaultBestLapTime = defaultBestLap;
	playerController->defaultBestRaceTime = defaultBestTime;
	playerController->maxLaps = maxLaps;
	playerController->MapName = mapName;
	playerController->saveSlot = SaveGameName;
	playerController->Setup();
}

void ATracker::SetupCheckpoints()
{
	for(int32 index=0; index<checkPoints.Num();index++)
	{
		checkPoints[index]->SetCheckpointIndex(index);
		checkPoints[index]->Deactivate();
		checkPoints[index]->CheckpointPassed.AddDynamic(this, &ATracker::OnCheckpointPassed);
	}
	totalCheckpoints = checkPoints.Num();
	checkPoints[0]->Activate();
}

void ATracker::LapCheck(int32 CheckpointPassed)
{
	playerController->respawnLocation = checkPoints[CheckpointPassed]->GetActorTransform();
	if (CheckpointPassed == (totalCheckpoints - 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("racecomplete"));
		if (!RaceCompleteCheck())
		{
			checkPoints[0]->Activate();
		}
		playerController->UpdateLap();
	}
	else
	{
		checkPoints[CheckpointPassed + 1]->Activate();
	}
}

bool ATracker::RaceCompleteCheck()
{
	bool result;
	result=(playerController->currentLap >= playerController->maxLaps);
	playerController->raceComplete = result;
	return result;
}

