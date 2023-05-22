// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnerLooped.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerLooped : public ASpawner
{
	GENERATED_BODY()

protected:
	void Tick(float DeltaTime) override;

	void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	bool RandomRespawn = false;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	FVector SpawningBox = FVector(200,200,200);
	
};
