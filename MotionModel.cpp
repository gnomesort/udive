// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionModel.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "EngineGlobals.h"
// #include "MotionControllerComponent.h"

// Sets default Values
AMotionModel::AMotionModel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMotionModel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMotionModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (MotionMode)
	{
	case ArmForceMode:
		ProcessArmForceMode();
		break;
	default:
		break;
	}
}

void AMotionModel::ProcessArmForceMode()
{
	float MomentumR = (RightControllerRelativeLocation).Size();
	float MomentumL = (LeftControllerRelativeLocation).Size();

	RSForceRight = RightControllerRSAxisUp * MovementAxisRight_Y;
	RSForceLeft = LeftControllerRSAxisUp * MovementAxisLeft_Y;

	// WSForceRight = -RightControllerWSAxisUp * MovementAxisRight_Y + RightControllerWSAxisRight * MovementAxisRight_X;
	// WSForceLeft = -LeftControllerWSAxisUp * MovementAxisLeft_Y + LeftControllerWSAxisRight * MovementAxisLeft_X;

	WSForceRight = RightControllerWSAxisForward * MovementAxisRight_Y + RightControllerWSAxisRight * MovementAxisRight_X;
	// WSForceLeft = LeftControllerWSAxisForward * MovementAxisLeft_Y + LeftControllerWSAxisRight * MovementAxisLeft_X;
	WSForceLeft = LeftControllerWSAxisForward * MovementAxisLeft_Y;

	FVector RRotAxis = FVector::CrossProduct(RightControllerRelativeLocation, RSForceRight);
	FVector LRotAxis = FVector::CrossProduct(LeftControllerRelativeLocation, RSForceLeft);
	
	float RAng = MomentumR * UKismetMathLibrary::Abs(MovementAxisRight_Y) * WRatio;
	float LAng = MomentumL * UKismetMathLibrary::Abs(MovementAxisLeft_Y) * WRatio;

	FRotator RRot = UKismetMathLibrary::RotatorFromAxisAndAngle(RRotAxis, RAng);
	FRotator LRot = UKismetMathLibrary::RotatorFromAxisAndAngle(LRotAxis, LAng);
	FRotator CRot = UKismetMathLibrary::ComposeRotators(RRot, LRot);

	

	CurrentIncRotator = CRot;
	// CurrentIncrementYaw = CRot.Yaw;
	// // CurrentIncrementPitch = CRot.Pitch;
	
	// // CurrentIncrementRoll = CRot.Roll;
	// DW = FVector(CRot.Yaw, 0.f, 0.f);
	DW = FVector(MovementAxisLeft_X, 0.f, 0.f);
	CurrentIncrementYaw += DW.X * WRatio;
	
	CurrentIncrementPitch = 0.f;
	CurrentIncrementRoll = 0.f;

	UKismetMathLibrary::FClamp(CurrentIncrementYaw, -MaxW, MaxW);
	CurrentIncrementYaw *= EnvironmentalResistance * (1.f - GrabAxisLeft) * (1.f - GrabAxisRight);


	DV = (WSForceLeft + WSForceRight) * VRatio * 0.1f;
	V += DV;
	if (V.Size() > MaxV)
		V * MaxV/V.Size();
	V *= EnvironmentalResistance * (1.f - GrabAxisLeft) * (1.f - GrabAxisRight);

	// CurrentLocation += V;
	float resp = -0.5f;
	if (CurrentLocation.X + V.X < LimitRangeMax.X && CurrentLocation.X + V.X > LimitRangeMin.X)
		CurrentLocation.X += V.X;
	else 
		V.X *= resp;

	if (CurrentLocation.Y + V.Y < LimitRangeMax.Y && CurrentLocation.Y + V.Y > LimitRangeMin.Y)
		CurrentLocation.Y += V.Y;
	else
		V.Y *= resp;

	if (CurrentLocation.Z + V.Z < LimitRangeMax.Z && CurrentLocation.Z + V.Z > LimitRangeMin.Z)
		CurrentLocation.Z += V.Z;
	else 
		V.Z *= resp;

	// if(GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Blue, 
	// 		FString::Printf(TEXT("[ %f %f %f ]"), CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z));	
	// }

	// CurrentLocation += (WSForceLeft + WSForceRight) * WRatio;
}

void AMotionModel::ProcessArmForceStabMode()
{

}

void AMotionModel::ProcessStickForceMode()
{

}


void AMotionModel::SetInitLocation(FVector Location)
{
	CurrentLocation = Location;
}

FVector AMotionModel::GetLocation()
{
	return CurrentLocation;
}

FVector AMotionModel::GetRotIncrementYawPitchRoll()
{
	return FVector(CurrentIncrementYaw, CurrentIncrementPitch, CurrentIncrementRoll);
}

FRotator AMotionModel::GetIncRotator()
{
	return CurrentIncRotator;
}


void AMotionModel::SetCameraAxis(FVector Forward, FVector Right, FVector Up)
{
	CameraAxisForward = Forward;
	CameraAxisRight = Right;
	CameraAxisUp = Up;
}


void AMotionModel::SetControllerWorldAxisLeft(FVector Forward, FVector Right, FVector Up)
{
	LeftControllerWSAxisForward = Forward;
	LeftControllerWSAxisRight = Right;
	LeftControllerWSAxisUp = Up;
}

void AMotionModel::SetControllerWorldAxisRight(FVector Forward, FVector Right, FVector Up)
{
	RightControllerWSAxisForward = Forward;
	RightControllerWSAxisRight = Right;
	RightControllerWSAxisUp = Up;
}

void AMotionModel::SetControllerRoomAxisLeft(FVector Forward, FVector Right, FVector Up)
{
	// LeftControllerRsAxisForward = Forward;
	// LeftControllerRsAxisRight = Right;
	// LeftControllerRsAxisUp = Up;
	LeftControllerRSAxisUp = FVector(-Up.Y, Up.X, -Up.Z);
}

void AMotionModel::SetControllerRoomAxisRight(FVector Forward, FVector Right, FVector Up)
{
	// RightControllerRSAxisForward = Forward;
	// RightControllerRSAxisRight = Right;
	// RightControllerRSAxisUp = Up;
	RightControllerRSAxisUp = FVector(Up.Y, Up.X, Up.Z);
}

void AMotionModel::SetControllerRelativeLocationLeft(FVector RelativeLocation)
{
	float ErrorOffsetX = 6.f;
	LeftControllerRelativeLocation = -BodyOffset + RelativeLocation;
	LeftControllerRelativeLocation.X += ErrorOffsetX;
}

void AMotionModel::SetControllerRelativeLocationRight(FVector RelativeLocation)
{
	RightControllerRelativeLocation = -BodyOffset + RelativeLocation;
}


void AMotionModel::SetTriggerAxisLeft(float Value)
{
	TriggerAxisLeft = Value;
}

void AMotionModel::SetTriggerAxisRight(float Value)
{
	TriggerAxisRight = Value;
}

void AMotionModel::SetGrabAxisLeft(float Value)
{
	GrabAxisLeft = Value;
}

void AMotionModel::SetGrabAxisRight(float Value)
{
	GrabAxisRight = Value;
}

void AMotionModel::SetMovementAxisRight_X(float Value)
{
	MovementAxisRight_X = Value;
}

void AMotionModel::SetMovementAxisLeft_X(float Value)
{
	MovementAxisLeft_X = Value;
}

void AMotionModel::SetMovementAxisRight_Y(float Value)
{
	MovementAxisRight_Y = Value;
}

void AMotionModel::SetMovementAxisLeft_Y(float Value)
{
	MovementAxisLeft_Y = Value;
}

FVector AMotionModel::DebugVector1() // blue
{	
	// return RightControllerRelativeLocation;
	// return FVector(CurrentIncrementYaw, CurrentIncrementPitch, CurrentIncrementRoll);
	return LeftControllerWSAxisRight;
}	

FVector AMotionModel::DebugVector2() // green
{
	return RightControllerWSAxisRight; 
}

FVector AMotionModel::DebugVector3() //  yellow
{
	return FVector(0.f, MovementAxisRight_X, MovementAxisLeft_X); 
	// return LeftControllerRelativeLocation;
	// float dp = FVector::DotProduct(RightControllerRelativeLocation, -RightControllerAxisUp);
	// float np = RightControllerRelativeLocation.Size() * RightControllerAxisUp.Size();
	// float ang = UKismetMathLibrary::Acos(dp/np);
	// return FVector(0.f, ang, FVector::CrossProduct(RightControllerRelativeLocation, -RightControllerAxisUp).Size());
}