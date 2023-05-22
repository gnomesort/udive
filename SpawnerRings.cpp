// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerRings.h"
#include "FlowObject.h"
#include "FlowModel.h"
#include "Kismet/KismetMathLibrary.h"
// #include "Public/LevelConfig.h"

ASpawnerRings::ASpawnerRings()
{
    SType = ST_RING;
}

void ASpawnerRings::BeginPlay()
{
	Super::BeginPlay();

    // if(GEngine){
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
    //             FString::Printf(TEXT("SpawnerID == %d"), SpawnerID));	
    // }
}

void ASpawnerRings::Restart(){
    TimeFromLast = 0;
	TimeFromStart = 0.f;
    BlockFirstEntry = true;
    Active = true;
}

void ASpawnerRings::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 

    // GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Yellow, 
	// 		            FString::Printf(TEXT("                 Enabled %d, Configured %d Active %d"), 
    //                         Enabled, 
    //                         Configured,
    //                         Active));

    if (!Enabled) return; 
    if (!Configured) return; if (!Active) return;

    // if (GEngine)
    // GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Yellow, 
	// 		            FString::Printf(TEXT("                 Rings ID %d , TimeFromStart %f, T %f  T+DT %f"), 
    //                         SpawnerID, 
    //                         TimeFromStart,
    //                         AConfig[0 * ConfigCols], 
    //                         AConfig[0 * ConfigCols] +  AConfig[0 * ConfigCols + 1]));	
    
    DisactivatePassedObjects();
    
    // TimeFromStart += DeltaTime;
    // TimeFromLast += DeltaTime;

    float TimeFromStartPrev = TimeFromStart;
    TimeFromStart = FlowModels[0]->GetCurrentTime();
    TimeFromLast += TimeFromStart - TimeFromStartPrev;


    float rangeRoll = 0.f;
    bool skip = true;
    for (int i = 0; i < ConfigRows; i++)
    {
        

        if (TimeFromStart + SpawnTimeOffset >=  AConfig[i * ConfigCols] && 
            TimeFromStart + SpawnTimeOffset <=  AConfig[i * ConfigCols] +  AConfig[i * ConfigCols + 1])
            {

                

                skip = false;
                if (BlockFirstEntry)
                {

                    GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Green, 
			            FString::Printf(TEXT("TimeFromStart + SpawnTimeOffset %f"), 
                            TimeFromStart + SpawnTimeOffset));	

                    StartRoll = AConfig[i * ConfigCols + 3];
                    StartSpin = AConfig[i * ConfigCols + 5];
                    StartScale = AConfig[i * ConfigCols + 8];

                    DRoll = AConfig[i * ConfigCols + 4];
	                DSpin = AConfig[i * ConfigCols + 6];
                    DScale = AConfig[i * ConfigCols + 9];

                    RandSpin = AConfig[i * ConfigCols + 7];
                    CurrTimeGap =  AConfig[i * ConfigCols + 2];
                    // if(GEngine)
                    // {
                    //     GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, 
	                // 		        FString::Printf(TEXT("ID = %d, %f %f %f %f %f %f %f %f %f %f"), 
                    //                 SpawnerID,
                    //                 AConfig[i * ConfigCols + 0],
                    //                 AConfig[i * ConfigCols + 1],
                    //                 AConfig[i * ConfigCols + 2],
                    //                 AConfig[i * ConfigCols + 3],
                    //                 AConfig[i * ConfigCols + 4],
                    //                 AConfig[i * ConfigCols + 5],
                    //                 AConfig[i * ConfigCols + 6],
                    //                 AConfig[i * ConfigCols + 7],
                    //                 AConfig[i * ConfigCols + 8],
                    //                 AConfig[i * ConfigCols + 9]
                    //                 ));	
                    // }
                    	
                }

                BlockFirstEntry = false;
                break;
            }
    }
    if (skip)
        BlockFirstEntry = true;

    // if (GEngine)
    // GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Yellow, 
	// 		            FString::Printf(TEXT("    TimeFromLast %f, CurrTimeGap %f, skip %d"), 
    //                         TimeFromLast, 
    //                         CurrTimeGap,
    //                         (int)skip));	

    if (!skip && TimeFromLast > CurrTimeGap)
    {
        FTransform Transform;
        FRotator RNormal(0.f, 0.f, 0.f);
        for (int i = 0; i < FlowModels.Num(); i++)
        {
            FTransform T0;
            FTransform TO; // Offset
            FRotator R0;
            FRotator RO; // Offset

            FlowModels[i]->GetTransformAndTilt(0, T0, R0);
            FlowModels[i]->GetTransformAndTilt(SpawnTimeOffset, TO, RO);

            FTransform dT = T0.Inverse() * TO;
            Transform = Transform * dT.Inverse();
            RNormal = UKismetMathLibrary::ComposeRotators(RNormal, RO);
        }

        StartRoll += DRoll;
        StartSpin += DSpin;
        StartScale *= DScale;

        FRotator roll(0.f, StartRoll, 0.f);
        FRotator spin;
        if (RandSpin > 1e-3)
            spin = FRotator(0.f, UKismetMathLibrary::RandomFloatInRange(0, RandSpin) - RandSpin * 0.5f, 0.f);
        else 
            spin = FRotator(0.f, StartSpin, 0.f);

        FTransform T = Transform;
        FVector tr = T.GetTranslation();
        T.SetLocation(tr);
        T.SetRotation(UKismetMathLibrary::ComposeRotators(roll, RNormal).Quaternion());
        T.SetScale3D(FVector(StartScale));
        
        RespawnObject(T, spin);

        TimeFromLast = 0.f;
    }
}

