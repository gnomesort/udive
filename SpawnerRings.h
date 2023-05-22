// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnerRings.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerRings : public ASpawner
{
	GENERATED_BODY()

public:
	ASpawnerRings();
	
	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float RollRange = 10.f;

	void Restart() override;

protected:
	
	
	float TimeFromLast = 0;
	float TimeFromStart = 0.f;
	float currentRoll = 0.f;
	float CurrTimeGap = 1.f;
	
	float CurrDevRange = 0.f;

	bool BlockFirstEntry = true;

	float StartScale = 1.f;
	float StartSpin = 1.f;
	float StartRoll = 1.f;

	float DScale = 1.f;
	float DSpin = 1.f;
	float DRoll = 1.f;

	float RandSpin = 1.f;


	void Tick(float DeltaTime) override;
	
	virtual void BeginPlay() override;
};
