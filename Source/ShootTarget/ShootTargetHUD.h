// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootTargetHUD.generated.h"

UCLASS()
class AShootTargetHUD : public AHUD
{
	GENERATED_BODY()

public:
	AShootTargetHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

