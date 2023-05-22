// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnerObstacleGate.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerObstacleGate : public ASpawner
{
	GENERATED_BODY()
public:
	ASpawnerObstacleGate();
	void Restart() override;

protected:

	float TimeFromStart = 0.f;
	float TimeFromLast = 0;
    float CurrTimeGap = 1.f;
    float CurrRoll = 0.f;
    float CurrRangeRoll = 0.f;

	bool BlockFirstEntry = true;

	FVector GateGapMin = FVector(100.f, 0.f, 0.f);
	FVector GateGapMax = FVector(400.f, 1000.f, 200.f);
	float GateDeviationRange = 20.f;

	void Tick(float DeltaTime) override;
};
