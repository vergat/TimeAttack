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
	SetupPlayerController(playerController);
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
	//ha bisogno del player controller finito per andare anvanti
}

void ATracker::SetupPlayerController(ATimeAttackPlayerController * TargetPlayer)
{
	//ha bisogno del player controller finito per andare anvanti
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
	//ha bisogno del player controller finito per andare anvanti
}

bool ATracker::RaceCompleteCheck()
{
	//ha bisogno del player controller finito per andare anvanti
	return false;
}

