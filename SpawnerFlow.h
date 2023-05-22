// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnerFlow.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerFlow : public ASpawner
{
	GENERATED_BODY()
public:

	float TimeFromLast = 0;
	float TimeFromStart = 0.f;
	float CurrentRoll = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float SpawnTimeGap = 1.f;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	FVector RandRange = FVector(800., 200., 300);

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float RollRange = 10.f;
	
protected:

	void Tick(float DeltaTime) override;

	// bool RespawnObject(FTransform Transform, FRotator Spin = FRotator()) override;
};
