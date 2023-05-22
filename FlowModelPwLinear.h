// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowModel.h"
#include "FlowModelPwLinear.generated.h"


typedef struct 
{
	FVector V;
	FVector R;
	float T;
} STrajectoryNode;

/**
 * 
 */
UCLASS()
class UDIVE_API AFlowModelPwLinear : public AFlowModel
{
	GENERATED_BODY()

public:
	AFlowModelPwLinear();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowParams")
	float VelocityScale = 500.f;

	int SetConfig(const FString& configContent);
	int SetConfig(const TArray<FString>& configLines);
	int SetConfig();
	
protected:

	bool LogConfig = true;
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	TArray<STrajectoryNode> TrajectoryNodes;

	// UFUNCTION(BlueprintCallable)
	// void AddTrajectoryNode(TVector<float, 4> Node);

	UFUNCTION(BlueprintCallable)
	void SetTrajectoryNodes();

	UFUNCTION(BlueprintCallable)
	void CalcTrajectoryNodes();

	FTransform GetGlobalTransform(float TimeFromNow, FVector P) override;
	
	FTransform GetIncrementTransform(float TimeFromNow, float DeltaTime, FVector P) override;	

	void GetTransformAndTilt(float TimeFromNow, FTransform & Transform, FRotator & 	Rotator, FVector * dR = NULL) override;
};
