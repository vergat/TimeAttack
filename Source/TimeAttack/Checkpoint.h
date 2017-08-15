// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include"TimeAttackPawn.h"
#include "Checkpoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckpointPassed,int32,checkpointIndex);

UCLASS()
class TIMEATTACK_API ACheckpoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckpoint();

private:
	UPROPERTY(EditAnywhere)
	int32 checkpointIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetCheckpointIndex(int32 index);
	int32 GetCheckPointIndex();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UArrowComponent* Arrow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* Trigger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UParticleSystemComponent* Particle;

		FCheckpointPassed CheckpointPassed;

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void Activate();

	UFUNCTION()
		void Deactivate();

};
