// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionModel.generated.h"

UENUM()
enum MMode 
{
	ArmForceMode = 0,
	ArmForceStabMode,
	StickForceMode
};

UCLASS()
class UDIVE_API AMotionModel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMotionModel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	TEnumAsByte<MMode> MotionMode = ArmForceMode;

	// UPROPERTY(EditAnywhere, Category = "Model Parameters")
	// FVector LimitRange = FVector(1000.f, 200.f, 200.f);

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	FVector LimitRangeMin = FVector(-500.f, -200.f, -15100.f);

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	FVector LimitRangeMax = FVector(500.f, 200.f, 15100.f);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector GetLocation();

	UFUNCTION(BlueprintCallable)
	void SetInitLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
	FVector GetRotIncrementYawPitchRoll();

	UFUNCTION(BlueprintCallable)
	FRotator GetIncRotator();

	// UFUNCTION(BlueprintCallable)
	// float GetIncrementYaw();

	// UFUNCTION(BlueprintCallable)
	// void AttachRightController(class UMotionControllerComponent* RController);

	UFUNCTION(BlueprintCallable)
	void SetCameraAxis(FVector Forward, FVector Right, FVector Up);
	
	UFUNCTION(BlueprintCallable)
	void SetControllerWorldAxisLeft(FVector Forward, FVector Right, FVector Up);

	UFUNCTION(BlueprintCallable)
	void SetControllerWorldAxisRight(FVector Forward, FVector Right, FVector Up);

	UFUNCTION(BlueprintCallable)
	void SetControllerRoomAxisLeft(FVector Forward, FVector Right, FVector Up);

	UFUNCTION(BlueprintCallable)
	void SetControllerRoomAxisRight(FVector Forward, FVector Right, FVector Up);



	UFUNCTION(BlueprintCallable)
	void SetControllerRelativeLocationLeft(FVector RelativeLocation);

	UFUNCTION(BlueprintCallable)
	void SetControllerRelativeLocationRight(FVector RelativeLocation);
	
	UFUNCTION(BlueprintCallable)
	void SetTriggerAxisLeft(float Value);

	UFUNCTION(BlueprintCallable)
	void SetTriggerAxisRight(float Value);

	UFUNCTION(BlueprintCallable)
	void SetGrabAxisLeft(float Value);

	UFUNCTION(BlueprintCallable)
	void SetGrabAxisRight(float Value);

	UFUNCTION(BlueprintCallable)
	void SetMovementAxisRight_X(float Value);

	UFUNCTION(BlueprintCallable)
	void SetMovementAxisLeft_X(float Value);

	UFUNCTION(BlueprintCallable)
	void SetMovementAxisRight_Y(float Value);

	UFUNCTION(BlueprintCallable)
	void SetMovementAxisLeft_Y(float Value);

	UFUNCTION(BlueprintCallable)
	FVector DebugVector1();

	UFUNCTION(BlueprintCallable)
	FVector DebugVector2();

	UFUNCTION(BlueprintCallable)
	FVector DebugVector3();

private:
	// FTransform MainTransform;
	// class UMotionControllerComponent * RightController;

	FVector CurrentLocation = FVector(0.f);
	float CurrentIncrementYaw;
	float CurrentIncrementPitch;
	float CurrentIncrementRoll;
	FRotator CurrentIncRotator;

	FVector CameraAxisForward;
	FVector CameraAxisRight;
	FVector CameraAxisUp;

	// Room Scale
	FVector LeftControllerRSAxisForward;
	FVector LeftControllerRSAxisRight;
	FVector LeftControllerRSAxisUp;
	FVector RightControllerRSAxisForward;
	FVector RightControllerRSAxisRight;
	FVector RightControllerRSAxisUp;

	// World Scale
	FVector LeftControllerWSAxisForward;
	FVector LeftControllerWSAxisRight;
	FVector LeftControllerWSAxisUp;
	FVector RightControllerWSAxisForward;
	FVector RightControllerWSAxisRight;
	FVector RightControllerWSAxisUp;

	FVector LeftControllerRelativeLocation;
	FVector RightControllerRelativeLocation;

	FVector V;
	FVector W;
	FVector DV;
	FVector DW;



	float TriggerAxisLeft;
	float TriggerAxisRight;
	float GrabAxisLeft;
	float GrabAxisRight;
	float MovementAxisRight_X;
	float MovementAxisLeft_X;
	float MovementAxisRight_Y;
	float MovementAxisLeft_Y;

	FVector RSForceRight;
	FVector RSForceLeft;
	FVector WSForceRight;
	FVector WSForceLeft;

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	FVector BodyOffset = FVector(0.f, 0.f, -30.f);

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	float WRatio  = 1e-3;

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	float VRatio  = 1.f;

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	float MaxV  = 2.f;

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	float MaxW  = 0.01f;

	UPROPERTY(EditAnywhere, Category = "Model Parameters")
	float EnvironmentalResistance  = 0.99f;


	void ProcessArmForceMode();
	void ProcessArmForceStabMode();
	void ProcessStickForceMode();
};
