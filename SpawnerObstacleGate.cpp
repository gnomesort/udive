// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerObstacleGate.h"
#include "FlowObject.h"
#include "FlowModel.h"
// #include "nlohmann/json.hpp"
#include "Kismet/KismetMathLibrary.h"
// #include "Public/LevelConfig.h"
// using json = nlohmann::json;

ASpawnerObstacleGate::ASpawnerObstacleGate()
{
    SType = ST_GATE;
}

void ASpawnerObstacleGate::Restart(){
    TimeFromLast = 0;
	TimeFromStart = 0.f;
    BlockFirstEntry = true;
    Active = true;
}

void ASpawnerObstacleGate::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 
    if (!Enabled) return; if (!Configured) return; if (!Active) return;
    DisactivatePassedObjects();
    
    TimeFromStart += DeltaTime;
    TimeFromLast += DeltaTime;
    bool skip = true;
    for (int i = 0; i < ConfigRows; i++)
    {
           
        if (TimeFromStart + SpawnTimeOffset >=  AConfig[i * ConfigCols] && 
            TimeFromStart + SpawnTimeOffset <=  AConfig[i * ConfigCols] +  AConfig[i * ConfigCols + 1])
            {
                skip = false;
                if (BlockFirstEntry)
                {
                    CurrTimeGap = AConfig[i * ConfigCols + 2];

                    CurrRangeRoll = AConfig[i * ConfigCols + 3];

                    GateGapMin = FVector(AConfig[i * ConfigCols + 4],
                                         AConfig[i * ConfigCols + 5],
                                         AConfig[i * ConfigCols + 6]);

                    GateGapMax = FVector(AConfig[i * ConfigCols + 7],
                                         AConfig[i * ConfigCols + 8],
                                         AConfig[i * ConfigCols + 9]);

                    GateDeviationRange = AConfig[i * ConfigCols + 10];

                    // UE_LOG(LogTemp, Warning, TEXT("BlockFirstEntry! min %f  %f  %f   max  %f  %f  %f  cfg_max %f %f %f"), 
                    //          SpawnerID,
                    //          GateGapMin.X,
                    //          GateGapMin.Y,
                    //          GateGapMin.Z,
                    //          GateGapMax.X,
                    //          GateGapMax.Y,
                    //          GateGapMax.Z);
                }

                BlockFirstEntry = false;
                break;
            }
    }

    if (skip)
        BlockFirstEntry = true;

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

        FVector gap = FVector(
            UKismetMathLibrary::RandomFloatInRange(GateGapMin.X, GateGapMax.X),
            UKismetMathLibrary::RandomFloatInRange(GateGapMin.Y, GateGapMax.Y),
            UKismetMathLibrary::RandomFloatInRange(GateGapMin.Z, GateGapMax.Z)); 

        FRotator rotA(
            UKismetMathLibrary::RandomFloatInRange(-GateDeviationRange, GateDeviationRange),
            UKismetMathLibrary::RandomFloatInRange(-GateDeviationRange, GateDeviationRange),
            UKismetMathLibrary::RandomFloatInRange(-GateDeviationRange, GateDeviationRange)); 

        FRotator rotB(
            UKismetMathLibrary::RandomFloatInRange(-GateDeviationRange, GateDeviationRange),
            UKismetMathLibrary::RandomFloatInRange(-GateDeviationRange, GateDeviationRange),
            UKismetMathLibrary::RandomFloatInRange(-GateDeviationRange, GateDeviationRange)); 

        CurrRoll += UKismetMathLibrary::RandomFloatInRange(-CurrRangeRoll, CurrRangeRoll);
        
        UE_LOG(LogTemp, Warning, TEXT("ASpawner ID %d (Gate): min %f  %f  %f   max  %f  %f  %f  cfg_max %f %f %f"), 
                             SpawnerID,
                             GateGapMin.X,
                             GateGapMin.Y,
                             GateGapMin.Z,
                             GateGapMax.X,
                             GateGapMax.Y,
                             GateGapMax.Z,
                             AConfig[7],
                             AConfig[8],
                             AConfig[9] );
        

        FTransform T = Transform;
        FVector tr = T.GetTranslation();
        tr.X += gap.X;
        tr.Y += gap.Y;
        tr.Z += gap.Z;
        T.SetLocation(tr);
        FRotator roll(0.f, CurrRoll, 0.f);
        FRotator fin = UKismetMathLibrary::ComposeRotators(roll, rotA); 
        T.SetRotation(UKismetMathLibrary::ComposeRotators(fin, RNormal).Quaternion());
        RespawnObject(T, FRotator(0.f, 0.f, 0.f));

        T = Transform;
        tr = T.GetTranslation();
        tr.X -= gap.X;
        tr.Y -= gap.Y;
        tr.Z -= gap.Z;
        T.SetLocation(tr);
        roll = FRotator(0.f, CurrRoll + 180.f, 0.f);
        fin = UKismetMathLibrary::ComposeRotators(roll, rotB); 
        T.SetRotation(UKismetMathLibrary::ComposeRotators(fin, RNormal).Quaternion());
        RespawnObject(T, FRotator(0.f, 0.f, 0.f));

        TimeFromLast = 0.f;
        // CurrTimeGap = UKismetMathLibrary::RandomFloatInRange(CurrTimeGapMin, CurrTimeGapMax);
    }
}