// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowModel.h"
#include "FlowModel_Controll.generated.h"

// UENUM(BlueprintType)
// enum class EKinematicType : uint8 {
//     KT_Linear   UMETA(DisplayName="Linear"),
//     KT_Force    UMETA(DisplayName="Force"),
//     KT_Inertia  UMETA(DisplayName="Inertia"),
// };

/**
 * 
 */
UCLASS()
class UDIVE_API AFlowModel_Controll : public AFlowModel
{
	GENERATED_BODY()

public:
	AFlowModel_Controll();
	FTransform GetIncrementTransform(float TimeFromNow, float DeltaTime, FVector P) override;	

	UFUNCTION(BlueprintCallable)
	void SetUpVector(FVector UpVectorArg);

	UFUNCTION(BlueprintCallable)
	void SetRightVector(FVector RightVectorArg);

	UFUNCTION(BlueprintCallable)
	void SetForwardVector(FVector ForwardVectorArg);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float SpeedArg);

	// UPROPERTY(EditAnywhere, Category = "Flow")
	// EKinematicType FlowType;
	
private:
	FVector UpVector;
	FVector RightVector;
	FVector ForwardVector;

	float Speed = 300.f;
};
