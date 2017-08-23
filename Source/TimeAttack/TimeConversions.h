// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class TIMEATTACK_API TimeConversions
{
public:
	static FText TimeToText(float time);
private:
	static FString TimeToSTring(int32 time);
};
