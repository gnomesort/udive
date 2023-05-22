// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomConfig.h"
#include "CoreMinimal.h"
#include "../Spawner.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "../FlowModel.h"

const FString MarioModeContent = "";


const FString FreeFallContent = "";


// Sets default values
ARoomConfig::ARoomConfig()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, "ARoomConfig::ARoomConfig()");	
}

void ARoomConfig::AddSpawner(class ASpawner * spawner)
{
	Spawners.Add(spawner);
}

void ARoomConfig::SetFlowModel(class AFlowModel * fmodel)
{
	FlowModel = fmodel;
}

void ARoomConfig::LocateRoomSpawners(FVector center)
{
	if (Spawners.Num() < 1){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
                FString::Printf(TEXT("StopRoomSpawners: Spawners.Num() < 1")));	

		return;
	} 	
	for (auto spawner : Spawners)
	{
		if (spawner)
			spawner->SetActorLocation(center);
	}
}

void ARoomConfig::StopRoomSpawners()
{
	if (Spawners.Num() < 1){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
                FString::Printf(TEXT("StopRoomSpawners: Spawners.Num() < 1")));	

		return;
	} 	
	for (auto spawner : Spawners)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
        //         FString::Printf(TEXT("Stop Spawner %d "), spawner->SpawnerID));	
		if (spawner)
			spawner->Stop();
	}
}

void ARoomConfig::RestartRoomSpawners()
{
	if (Spawners.Num() < 1){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
                FString::Printf(TEXT("StopRoomSpawners: Spawners.Num() < 1")));	

		return;
	} 	
	for (auto spawner : Spawners)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
                FString::Printf(TEXT("Start Spawner %d "), spawner->SpawnerID));	
		if (spawner)
			spawner->Restart();
	}

	FlowModel->Restart();
}

void ARoomConfig::SetSpawnersConfig()
{
	for (auto spawner : Spawners){
		spawner->SetConfig(ConfigContent);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                FString::Printf(TEXT(" Spawners Config : nSpawnerID = %d, config cols = %d"), 
									spawner->SpawnerID, spawner->GetConfigCols()));	
	}
}

void ARoomConfig::SetFlowModelConfig()
{
	if (FlowModel){
		FlowModel->SetConfig(ConfigContent);
		// FlowModel->SetConfig(ConfigLines);
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                FString::Printf(TEXT(" 				 !!! FlowModel NULL")));	
	}
}

void ARoomConfig::SetConfig(FString fname)
{
	ConfigFileName = fname;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, 
        FString::Printf(TEXT(" 				PLATFORM : %s"), *UGameplayStatics::GetPlatformName() ));	

	if (UGameplayStatics::GetPlatformName().Contains("111Windows")){
		FString FilePath = FPaths::ProjectContentDir() + fname;
    	FFileHelper::LoadFileToString(ConfigContent, *FilePath);
	} else {
		if (ConfigFileName.Contains("ario")){
			// ConfigContent = MarioModeContent;
			ConfigContent = "\n\
FMT_PWLINEAR 10 \n\
COLS 4 \n\
ROWS 4 \n\
 Vx    Vy    Vz   Dur \n\
0.f,  0.f,  -1.f,   15.f \n\
0.f,  0.f,  -1.f,   15.f \n\
0.f,  0.f,  -1.5f,  15.f \n\
0.f,  0.f,  -3.5f,  15.f \n\
\n\
ST_RING 13 \n\
COLS 10 \n\
ROWS 1 \n\
  T     Dur    TGap    startRoll dRoll  startSpin   dSpin  randSpin  startScale  ratioScale \n\
31.,   90.,     2.0,      0.,     0.,     0.,       0.0,     1.2,        1.,        1. \n\
\n\
ST_RING 10 \n\
COLS 10 \n\
ROWS 1 \n\
  T     Dur    TGap    startRoll dRoll  startSpin   dSpin  randSpin  startScale  ratioScale \n\
4.,   25.,     2.0,      0.,     180.,     0.,       0.0,     0.1,        3.,        1. \n\
";

			GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Purple, 
        		FString::Printf(TEXT("%s"), *ConfigContent ));	

		} else{
			// ConfigContent = FreeFallContent;
			ConfigContent = "\n\
FMT_PWLINEAR 20 \n\
COLS 4 \n\
ROWS 9 \n\
 Vx    Vy    Vz   Dur \n\
0.f,  0.f,  -1.f,   35.f \n\
0.f,  0.f,  -1.f,   5.f \n\
0.f,  1.f,  -1.f,   20.f \n\
0.f,  1.f,  -1.f,   5.f \n\
-1.f,  0.f,  -1.f,   10.f \n\
-1.f,  0.f,  -1.f,   10.f \n\
0.f,  0.f,  -1.5f,   10.f \n\
0.f,  0.f,  -1.5f,   3.f \n\
0.f,  0.f,  -0.0001f,  1000.f \n\
\n\
ST_GATE 20 \n\
COLS 11 \n\
ROWS 2 \n\
 T     Dur      TGap    AngGap       GapMin              XGapMa          Dev   \n\
20.f,  10.f,    1.5,    90.f,     0.f,  0.f,  0.f,    0.f, 0.f, 0.f,     20.f \n\
80.f, 10.f,    1.5,    90.f,     0.f,  0.f,  0.f,    0.f, 0.f, 0.f,     20.f \n\
\n\
 --- Simple Rings --- \n\
ST_RING 20 \n\
COLS 10 \n\
ROWS 3 \n\
 T     Dur    TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale \n\
5.,   10.,    0.3,       0.,     0.,      0.,    0.0,      10.,     10.,       1. \n\
21,    5,     0.1,       0.,     10.,     0.,    0.0,      0.,      5.,        1. \n\
43,    28,    1.25,       0.,     30.,     0.,    0.0,      5.,      15.,       1. \n\
\n\
 --- Simple Rings --- \n\
ST_RING 25 \n\
COLS 10 \n\
ROWS 1 \n\
 T     Dur    TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale \n\
7.,   30.,    1.7,       0.,     0.,      0.,    0.0,      10.,     40.,       1. \n\
\n\
  --- Man --- \n\
ST_RING 21 \n\
COLS 10 \n\
ROWS 1 \n\
 T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale  \n\
30.,  5.1,      1.0,       0.,      0.,      0.,       0.0,   0.,        3.,        1. \n\
 \n\
  --- Arrow --- \n\
ST_RING 22 \n\
COLS 10 \n\
ROWS 1 \n\
 T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale  \n\
60.,  4.,        0.65,      0.,     0.,       00.,       0.0,   0.,        10.,        1. \n\
 \n\
  --- Dots 1 --- \n\
ST_RING 23 \n\
COLS 10 \n\
ROWS 1 \n\
 T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale  \n\
1.,    120.,     0.65,      0.,     10.,       00.,       0.0,   0.,        1.,        1. \n\
\n\
  --- Dots 2 --- \n\
ST_RING 24 \n\
COLS 10 \n\
ROWS 1 \n\
T     Dur       TGap    startRoll dRoll  startSpin  dSpin   randSpin  startScale  ratioScale  \n\
1.,    120.,     0.85,      0.,     17.,       00.,       0.0,   0.1,        2.,        1. \n\
\n\
\n\
ST_MBLOCK 20 \n\
COLS 5 \n\
ROWS 1 \n\
 T     Dur      TGap  AngGap   dev  \n\
35.f,    20.f,    0.2,   4.f,   20.f \n\
 \n\
ST_VBLOCK 20 \n\
COLS 13 \n\
ROWS 2 \n\
 T     Dur     TGap   minSc: X    Y     Z     maxSc: X      Y     Z    Margine:  X     Y     rage:  X       Y   \n\
10.f,  10.f,  1.0f,         1.f, 1.f,  20.0f,       1.f,   1.f,  20.0f,        700.f,  700.f,      1500.f,  1500.f \n\
95.f,  30.f,  1.0f,         1.f, 1.f,  20.0f,       1.f,   1.f,  20.0f,        700.f,  700.f,      1500.f,  1500.f \n\
\n\
ST_SBLOCK 20 \n\
COLS 8 \n\
ROWS 1 \n\
 T     Dur     TGap    MinScale   MaxScale  0-Margine  SpawnRage  SpinRange \n\
0.f,   120.f,  2.0f,      10.f,        10.f,     3000.f,    3000.f,    1.0 \n\
";
			// GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Purple, 
        	// 		FString::Printf(TEXT("%s"), *ConfigContent ));	
		}
	}

}



FString ARoomConfig::GetConfig()
{
	// if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, "GetConfig");	

	// TArray<FString> Lines;
    // ConfigContent.ParseIntoArrayLines(Lines);
 	// for (FString Line : Lines)
    // {
    //     if(GEngine)
	//      GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, Line);	
    // }

	return ConfigContent;
	// return ConfigFileName;
}

// Called when the game starts or when spawned
void ARoomConfig::BeginPlay()
{
	Super::BeginPlay();
	SetConfig(ConfigFileName);

	float TimeToSetConfig = 0.2f;

	FTimerHandle TimerHandleSP;
    FTimerDelegate TimerDelegateSP;
    TimerDelegateSP.BindUFunction(this, FName("SetSpawnersConfig"));
    GetWorld()->GetTimerManager().SetTimer(TimerHandleSP, TimerDelegateSP, TimeToSetConfig, false);

	FTimerHandle TimerHandleFM;
    FTimerDelegate TimerDelegateFM;
    TimerDelegateFM.BindUFunction(this, FName("SetFlowModelConfig"));
    GetWorld()->GetTimerManager().SetTimer(TimerHandleFM, TimerDelegateFM, TimeToSetConfig, false);
}

// Called every frame
void ARoomConfig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

