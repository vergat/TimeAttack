// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeAttack.h"
#include "Checkpoint.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Root);
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
	Trigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACheckpoint::OnOverlapEnd);
}

void ACheckpoint::SetCheckpointIndex(int32 index)
{
	checkpointIndex = index;
}

int32 ACheckpoint::GetCheckPointIndex()
{
	return checkpointIndex;
}

void ACheckpoint::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ATimeAttackPawn* Pawn = Cast<ATimeAttackPawn>(OtherActor);
	if (FVector::DotProduct(Pawn->GetVelocity(),Arrow->GetComponentRotation().Vector())>0.f) {
		CheckpointPassed.Broadcast(checkpointIndex);
		Deactivate();
	}
}

void ACheckpoint::Activate()
{
	Particle->SetHiddenInGame(false, false);
	Trigger->SetHiddenInGame(false, false);
	Trigger->bGenerateOverlapEvents = true;
}

void ACheckpoint::Deactivate()
{
	Particle->SetHiddenInGame(true,false);
	Trigger->SetHiddenInGame(true,false);
	Trigger->bGenerateOverlapEvents = false;
}

