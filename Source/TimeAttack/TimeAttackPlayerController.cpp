// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeAttack.h"
#include "TimeAttackPawn.h"
#include "TimeAttackSaveGame.h"
#include "TimeConversions.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimeAttackPlayerController.h"

ATimeAttackPlayerController::ATimeAttackPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	raceTimeline=CreateDefaultSubobject<UTimelineComponent>(TEXT("raceTimeline"));
	lapTimeline= CreateDefaultSubobject<UTimelineComponent>(TEXT("LapTimeline"));
	RaceFunction.BindUFunction(this, FName("RaceTimelineFloatReturn"));
	LapFunction.BindUFunction(this, FName("LapTimelineFloatReturn"));
	GameHUD = false;
	SplashScreen = true;
	CountdownScreen = false;
}

void ATimeAttackPlayerController::BeginPlay()
{
	Super::BeginPlay();
	vehicle = Cast<ATimeAttackPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (HUDWidgetClass != nullptr)
	{
		currentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (currentWidget != nullptr)
		{
			currentWidget->AddToViewport();
		}
	}
	currentLap = 1;
	doOnceGuard = false;
	raceStart = false;
	InputComponent->BindAction("ResetBestTime", IE_Pressed, this, &ATimeAttackPlayerController::ResetBestTime);
	InputComponent->BindAction("StartSequence", IE_Released, this, &ATimeAttackPlayerController::StartRaceSequence);
	if (timeCurve)
	{
		raceTimeline->AddInterpFloat(timeCurve, RaceFunction,FName("alpha"));
		lapTimeline->AddInterpFloat(timeCurve, LapFunction, FName("LapTime"));
		
		raceTimeline->SetLooping(false);
		raceTimeline->SetIgnoreTimeDilation(true);
		lapTimeline->SetLooping(false);
		lapTimeline->SetIgnoreTimeDilation(true);
	}
}

void ATimeAttackPlayerController::RespawnVehicle()
{
	ATimeAttackPawn* newVehicle=GetWorld()->SpawnActor<ATimeAttackPawn>(respawnLocation.GetLocation(),respawnLocation.GetRotation().Rotator());
	Possess(newVehicle);
}

void ATimeAttackPlayerController::UpdateLap()
{
	if (!raceComplete) 
	{
		currentLap++;
		CurrentLapText = FText::FromString(FString::FromInt(currentLap));
	}
	EndLapUpdate();
}

void ATimeAttackPlayerController::EndLapUpdate()
{
	lapTimeline->Stop();
	if (currentLapTime <= bestLapTime)
	{
		bestLapTime = currentLapTime;
		BestLapTimeText = TimeConversions::TimeToText(bestLapTime);
		SaveGame();
	}
	currentLapTime = 0.0f;
	if (raceComplete)
	{
		EndRaceUpdate();
	}
	else
	{
		lapTimeline->PlayFromStart();
	}
}

void ATimeAttackPlayerController::EndRaceUpdate()
{
	GameHUD = false;
	SplashScreen = true;
	if (currentRaceTime < bestRaceTime)
	{
		bestRaceTime = currentRaceTime;
		BestRaceTimeText = TimeConversions::TimeToText(bestRaceTime);
		SaveGame();
	}
}

void ATimeAttackPlayerController::Restat()
{
	raceStart = false;
	GetWorldTimerManager().SetTimer(delay,this, &ATimeAttackPlayerController::RestartLevel, 5.0f, false);
}

void ATimeAttackPlayerController::Setup()
{
	SaveGameCheck();
	RefreshHUD();
}

void ATimeAttackPlayerController::RefreshHUD()
{
	CurrentLapText= FText::FromString(FString::FromInt(currentLap));
	MaxLapsText= FText::FromString(FString::FromInt(maxLaps));
	GoldTimeText = TimeConversions::TimeToText(goldTime);
	SilverTimeText = TimeConversions::TimeToText(silverTime);
	BronzeTimeText = TimeConversions::TimeToText(bronzeTime);
	BestLapTimeText = TimeConversions::TimeToText(bestLapTime);
	BestRaceTimeText = TimeConversions::TimeToText(bestRaceTime);
}

void ATimeAttackPlayerController::SaveGameCheck()
{
	if (UGameplayStatics::DoesSaveGameExist(saveSlot, 0))
	{
		LoadGame();
	}
	else
	{
		bestLapTime = defaultBestLapTime;
		bestRaceTime = defaultBestRaceTime;
		SaveGame();
	}
}

void ATimeAttackPlayerController::SaveGame()
{
	UTimeAttackSaveGame* instanceSaveGame = Cast<UTimeAttackSaveGame>(UGameplayStatics::CreateSaveGameObject(UTimeAttackSaveGame::StaticClass()));
	instanceSaveGame->SavedBestLapTime = bestLapTime;
	instanceSaveGame->SavedBestRaceTime = bestRaceTime;
	UGameplayStatics::SaveGameToSlot(instanceSaveGame, saveSlot, 0);
}

void ATimeAttackPlayerController::LoadGame()
{
	UTimeAttackSaveGame* instanceSaveGame = Cast<UTimeAttackSaveGame>(UGameplayStatics::CreateSaveGameObject(UTimeAttackSaveGame::StaticClass()));
	instanceSaveGame = Cast<UTimeAttackSaveGame>(UGameplayStatics::LoadGameFromSlot(saveSlot, 0));
	bestLapTime = instanceSaveGame->SavedBestLapTime;
	bestRaceTime = instanceSaveGame->SavedBestRaceTime;
}

void ATimeAttackPlayerController::ResetBestTime()
{
	if (!raceStart)
	{
		bestLapTime = defaultBestLapTime;
		bestRaceTime = defaultBestRaceTime;
		SaveGame();
		RefreshHUD();
	}
}

void ATimeAttackPlayerController::StartRaceSequence()
{
	if (!doOnceGuard)
	{
		SplashScreen = false;
		CountdownScreen = true;
		doOnceGuard = true;
		CountdownText = FText::FromString("3");
		timerDel.BindUFunction(this, FName("CountdownSequence"), 3);
		GetWorldTimerManager().SetTimer(delay, timerDel, 1.5f, false);
	}
}

void ATimeAttackPlayerController::RaceTimelineFloatReturn(float value)
{
	currentRaceTime = value;
	CurrentRaceTimeText = TimeConversions::TimeToText(value);
}

void ATimeAttackPlayerController::LapTimelineFloatReturn(float value)
{
	currentLapTime = value;
	CurrentLapTimeText = TimeConversions::TimeToText(value);
}

void ATimeAttackPlayerController::CountdownSequence(int32 value)
{
	if (value > 0)
	{
		CountdownText = FText::FromString(FString::FromInt(value));
		value--;
		timerDel.BindUFunction(this, FName("CountdownSequence"), value);
		GetWorldTimerManager().SetTimer(delay, timerDel, 1.0f, false);
	}
	else
	{
		CountdownText = FText::FromString("GO!");
		GetWorldTimerManager().SetTimer(delay, this, &ATimeAttackPlayerController::EndSequence, 1.0f, false);
	}
}

void ATimeAttackPlayerController::EndSequence()
{
	raceStart = true;
	CountdownScreen = false;
	GameHUD = true;
	raceTimeline->Play();
	lapTimeline->PlayFromStart();
}

void ATimeAttackPlayerController::RestartLevel()
{
	GetWorld()->Exec(GetWorld(), TEXT("RestartLevel"));
}

