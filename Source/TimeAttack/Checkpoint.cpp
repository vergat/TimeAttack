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
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(("Particle"));
	Particle->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoint::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ATimeAttackPawn* Pawn = Cast<ATimeAttackPawn>(OtherActor);
	if (FVector::DotProduct(Pawn->GetVelocity(),Arrow->GetComponentRotation().Vector())>0.f) {
		Deactivate();
	}
	//aggiungere eventdispatcher ChecpointPassed in tracker

}

void ACheckpoint::Activate()
{
	Particle->SetHiddenInGame(true, false);
	Trigger->SetHiddenInGame(true, false);
	Trigger->bGenerateOverlapEvents = true;
}

void ACheckpoint::Deactivate()
{
	Particle->SetHiddenInGame(true,false);
	Trigger->SetHiddenInGame(true,false);
	Trigger->bGenerateOverlapEvents = false;
}

