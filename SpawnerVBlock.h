// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnerVBlock.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerVBlock : public ASpawner
{
	GENERATED_BODY()

public:
	ASpawnerVBlock();
	void Restart() override;

protected:
	
	float TimeFromLast = 0;
	float TimeFromStart = 0;
	float CurrTimeGap = 1.f;
	float CurrDevRange = 0.f;

	FVector ZeroMargin;
	FVector Scale;
    FVector SpawnRange;

	bool BlockFirstEntry = true;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
};
