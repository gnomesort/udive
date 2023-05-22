// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowModel.h"
#include "FlowObject.h"
#include "Spawner.h"
#include "RoomConfig.h"

// Sets default values
AFlowModel::AFlowModel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("Hi from AFlowModel::AFlowModel()"));
}

int AFlowModel::SetConfig(const FString& configContent){
    return 0;
}

int AFlowModel::SetConfig(const TArray<FString>& configLines){
    return 0;
}

float AFlowModel::GetCurrentTime()
{
	return CurrentTime;
}

void AFlowModel::GetTransformAndTilt(float TimeFromNow, FTransform & Transform, FRotator & Rotator, FVector * dR)
{
	return;
}

// Called when the game starts or when spawned
void AFlowModel::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParams;
	// if (AFlowObject_Custom_BP)
    Helper.Add(GetWorld()->SpawnActor<AFlowObject>(AFlowObject_Custom_BP, FTransform(), SpawnParams));
	Helper.Add(GetWorld()->SpawnActor<AFlowObject>(AFlowObject_Custom_BP, FTransform(), SpawnParams));
	
	Helper[0]->SetActorHiddenInGame(true);
	Helper[1]->SetActorHiddenInGame(true);

	if (RoomConfig)
		RoomConfig->SetFlowModel(this);
}

void AFlowModel::AddSpawner(ASpawner * spawner)
{
	Spawners.Add(spawner);
}

void AFlowModel::Restart()
{
    if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.5, FColor::Red, "FlowModel Restart");
    CurrentTime = 0.f;
	SpeedOfTime = 1.f;
}

void AFlowModel::RestartWithSpawners()
{
    if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.5, FColor::Red, "FlowModel Restart");
    CurrentTime = 0.f;
	SpeedOfTime = 1.f;

	for (auto& spawner : Spawners){
		spawner->Restart();
	}
}

void AFlowModel::Pause()
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.5, FColor::Red, "Pause");
	SpeedOfTime = 0.f;
}

void AFlowModel::Resume()
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.5, FColor::Red, "Resume");
	SpeedOfTime = 1.f;
}

FTransform AFlowModel::GetGlobalTransform(float TimeFromNow, FVector P)
{
	return FTransform();
}

FTransform AFlowModel::GetIncrementTransform(float TimeFromNow, float DeltaTime, FVector P)
{
	return FTransform();
}

// Called every frame
void AFlowModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentTime += DeltaTime * SpeedOfTime;
}

