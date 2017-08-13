// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include"TimeAttackPawn.h"
#include "Checkpoint.generated.h"


UCLASS()
class TIMEATTACK_API ACheckpoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckpoint();

private:
	int32 CheckpointIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UArrowComponent* Arrow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* Trigger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UParticleSystemComponent* Particle;

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void Activate();

	UFUNCTION()
		void Deactivate();

};
