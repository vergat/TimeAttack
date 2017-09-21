// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeAttack.h"
#include "TimeConversions.h"

FText TimeConversions::TimeToText(float time)
{
	int32 minutes = FMath::FloorToInt(time / 60.0f);
	int32 seconds = FMath::FloorToInt(FMath::Fmod( time,60.0f));
	int32 milliseconds=FMath::FloorToInt(time);
	milliseconds= FMath::FloorToInt((time-milliseconds)*100.0f);
	FString minutesText=TimeToSTring(minutes);
	FString secondsText=TimeToSTring(seconds);
	FString millisecondsText=TimeToSTring(milliseconds);
	FString result = minutesText + ":" + secondsText + "." + millisecondsText;
	return FText::FromString(result);
}

FString TimeConversions::TimeToSTring(int32 time)
{
	FString result;
	if (time > 9)
	{
		result = FString::FromInt(time);
	}
	else
	{
		result="0"+ FString::FromInt(time);
	}
	return result;
}

