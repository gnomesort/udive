// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnerSBlock.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerSBlock : public ASpawner
{
	GENERATED_BODY()

public:
	ASpawnerSBlock();
	void Restart() override;

protected:
	
	float TimeFromLast = 0;
	float TimeFromStart = 0;
	float CurrTimeGap = 1.f;
	float CurrDevRange = 0.f;

	float SpinRagne = 0.f;
	float ZeroMargin;
	float Scale = 1.f;
    FVector SpawnRange;

	bool BlockFirstEntry = true;

	void Tick(float DeltaTime) override;	
};
