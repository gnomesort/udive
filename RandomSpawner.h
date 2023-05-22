// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "RandomSpawner.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ARandomSpawner : public ASpawner
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float SpawnTimeGap = 0.1;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float DisactivateDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float DisactivateScale = 2.f;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float SpawnRange = 300.f;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float ZeroMargine = 20.f;

	void Tick(float DeltaTime) override;

	// bool RespawnObject(FTransform Transform) override;

	bool TimeToDisactive(TWeakObjectPtr<class AFlowObject> FObject) override;

};
