// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "TimeAttackHud.generated.h"


UCLASS(config = Game)
class ATimeAttackHud : public AHUD
{
	GENERATED_BODY()

public:
	ATimeAttackHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};
