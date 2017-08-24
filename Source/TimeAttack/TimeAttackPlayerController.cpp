// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeAttack.h"
#include "TimeAttackPawn.h"
#include "TimeAttackSaveGame.h"
#include "TimeConversions.h"
#include "TimeAttackPlayerController.h"

ATimeAttackPlayerController::ATimeAttackPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curvy(TEXT("/Game/TimeAttack/TimeCurve"));
	if (Curvy.Object)
	{
		timeCurve = Curvy.Object;
	}
	raceTimeline = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("RacelineScore"));
	lapTimeline = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("LapTimeline"));
	RaceFunction.BindUFunction(this, FName("RaceTimelineFloatReturn"));
	LapFunction.BindUFunction(this, FName("LapTimelineFloatReturn"));
}


void ATimeAttackPlayerController::BeginPlay()
{
	vehicle = Cast<ATimeAttackPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (HUDWidgetClass != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("dentro hud"));
		currentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (currentWidget != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("widget"));
			currentWidget->AddToViewport();
		}
	}
	currentLap = 1;
	doOnceGuard = false;
	InputComponent->BindAction("PressingR", IE_Released, this, &ATimeAttackPlayerController::ResetBestTime);
	InputComponent->BindAction("StartSequence", IE_Released, this, &ATimeAttackPlayerController::StartRaceSequence);
	//raceTimeline = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("TimelineScore"));
	/*lapTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LapTimeline"));

	RaceFunction.BindUFunction(this, FName("RaceTimelineFloatReturn"));
	LapFunction.BindUFunction(this, FName("LapTimelineFloatReturn"));
	/*static ConstructorHelpers::FObjectFinder<UCurveFloat> Curvy(TEXT("/Game/TimeAttack/TimeCurve.uasset"));
	if (Curvy.Object)
	{
		timeCurve = Curvy.Object;
	}*/
	if (timeCurve)
	{
		UE_LOG(LogTemp, Warning, TEXT("cuuurvaaaa"));
		raceTimeline->AddInterpFloat(timeCurve, RaceFunction,FName("RaceTime"));
		lapTimeline->AddInterpFloat(timeCurve, LapFunction, FName("LapTime"));

		raceTimeline->SetLooping(false);
		lapTimeline->SetLooping(false);
	}
}

void ATimeAttackPlayerController::RespawnVehicle()
{
	ATimeAttackPawn* newVehicle=GetWorld()->SpawnActor<ATimeAttackPawn>(respawnLocation.GetLocation(),respawnLocation.GetRotation().Rotator());
	Possess(newVehicle);
}

void ATimeAttackPlayerController::UpdateLap()
{
	UE_LOG(LogTemp, Warning, TEXT("lapfinito"));

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
	//UE_LOG(LogTemp, Warning, TEXT("raceComplete %s"), (raceComplete ? TEXT("True") : TEXT("False")));
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
	}
}

void ATimeAttackPlayerController::StartRaceSequence()
{
	if (!doOnceGuard)
	{
		doOnceGuard = true;
		//todo hud init
		CountdownText = FText::FromString("3");
		timerDel.BindUFunction(this, FName("CountdownSequence"), 3);
		GetWorldTimerManager().SetTimer(delay, timerDel, 1.5f, false);
	}
}

void ATimeAttackPlayerController::RaceTimelineFloatReturn(float value)
{
	currentRaceTime = value;
	CurrentRaceTimeText = TimeConversions::TimeToText(value);
	UE_LOG(LogTemp, Warning, TEXT("racetime %f"),value);
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
	//todo HUD init
	raceStart = true;
	raceTimeline->Play();
	lapTimeline->PlayFromStart();
}

void ATimeAttackPlayerController::RestartLevel()
{
	GetWorld()->Exec(GetWorld(), TEXT("RestartLevel"));
}

