// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnerMBlock.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerMBlock : public ASpawner
{
	GENERATED_BODY()

public:
	ASpawnerMBlock();
	void Restart() override;
	
protected:

	float TimeFromLast = 0;
	float TimeFromStart = 0.f;
	float CurrTimeGap = 1.f;
	
	float CurrDevRange = 0.f;
	int switcher = 0;

	bool BlockFirstEntry = true;

	void Tick(float DeltaTime) override;

	// bool RespawnObject(FTransform Transform, FRotator Spin = FRotator()) override;
};
