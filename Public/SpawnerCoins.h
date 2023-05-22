// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Spawner.h"
#include "SpawnerCoins.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API ASpawnerCoins : public ASpawner
{
	GENERATED_BODY()
public:

	ASpawnerCoins();

	float TimeFromLast = 0;
	float TimeFromStart = 0;
	float CurrTimeGap = 1.f;
	float CurrDevRange = 0.f;
	float SpinRagne = 0.f;
	bool BlockFirstEntry = true;

	UPROPERTY(EditAnywhere, Category = "Param")
	float RangeScale = 0.7f;


	FVector StartOffset;
	FVector EndOffset;
	FVector Step;
	
protected:

	void Tick(float DeltaTime) override;
};
