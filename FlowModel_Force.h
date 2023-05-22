// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowModel.h"
#include "FlowModel_Force.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API AFlowModel_Force : public AFlowModel
{
	GENERATED_BODY()
public:

	AFlowModel_Force();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowParams")
	FVector StartDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowParams")
	float StartVelocity = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowParams")
	float HFactor = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowParams")
	float VFactor = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowParams")
	float HAmp = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowParams")
	float VAmp = 100.f;

	UPROPERTY(EditAnywhere, Category = "FlowParams")
	float RollTimeRatio= 0.f;

	UPROPERTY(EditAnywhere, Category = "FlowParams")
	float ScaleTimeRatio = 0.f;

	UPROPERTY(EditAnywhere, Category = "FlowParams")
	float OrtoFactor = 0.8;

	FTransform GetGlobalTransform(float TimeFromNow, FVector P) override;
	FTransform GetIncrementTransform(float TimeFromNow, float DeltaTime, FVector P) override;	

protected:
	void Tick(float DeltaTime) override;

private:
	int BasisFactorNum = 10;
	TArray<FVector> BasisFactor;
};
