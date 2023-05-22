// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowObject.h"
#include "FlowObject_Obstacle.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API AFlowObject_Obstacle : public AFlowObject
{
	GENERATED_BODY()
public:

	AFlowObject_Obstacle();

	UFUNCTION(BlueprintCallable)
	float GetGlowingValue();
	

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	float GlowAmp = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	float GlowMin = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	float GlowFrequencyMax = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	float GlowFrequencyMin = 1.f;
protected:
	// UPROPERTY(EditDefaultsOnly, Category = "Material")
	float Glowing = 10.f;
	float Phase = 0.f;
	float GlowFrequency = 2.f;
	

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
