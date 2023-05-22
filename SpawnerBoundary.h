// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnerBoundary.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerBoundary : public ASpawner
{
	GENERATED_BODY()

public:
	ASpawnerBoundary();
	
	UFUNCTION(BlueprintCallable)
	void SetGuardianPoints(TArray<FVector> Points);

protected:

	void Tick(float DeltaTime) override;

	TArray<FVector> GuardianPoints;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float SpawnTimeGap = 1.f;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	int BndSpawnNum = 10;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	int SimBndNum = 100;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
    float SimBndRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	int SimBndShift = 5;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	bool SimBoundary = true;
};
