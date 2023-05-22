// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowObject_Obstacle.h"
#include "Kismet/KismetMathLibrary.h"

AFlowObject_Obstacle::AFlowObject_Obstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Phase = UKismetMathLibrary::RandomFloatInRange(0.f, 100.f);
	GlowFrequency = UKismetMathLibrary::RandomFloatInRange(GlowFrequencyMin, GlowFrequencyMax);
}

void AFlowObject_Obstacle::BeginPlay()
{
	Super::BeginPlay();
}

float AFlowObject_Obstacle::GetGlowingValue()
{
	Glowing = GlowMin + GlowAmp * 0.5f + GlowAmp * 0.5f * UKismetMathLibrary::Sin(this->GetGameTimeSinceCreation() * GlowFrequency + Phase);
	return Glowing;
}