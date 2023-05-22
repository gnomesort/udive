// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerCoins.h"
#include "../FlowObject.h"
#include "../FlowModel.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Public/LevelConfig.h"

ASpawnerCoins::ASpawnerCoins()
{
    // Config = &LevelConfig::SPN_COIN_0_Config[0];
    // ConfigCols = LevelConfig::SPN_COIN_Col;
    // ConfigRows = sizeof(LevelConfig::SPN_COIN_0_Config)/(sizeof(float) * ConfigCols);
}

void ASpawnerCoins::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 
    if (!Enabled) return; if (!Configured) return; if (!Active) return;
    DisactivatePassedObjects();
    
    TimeFromStart += DeltaTime;
    TimeFromLast += DeltaTime;
    
    bool skip = true;
    FRotator spin;
    for (int i = 0; i < ConfigRows; i++)
    {
        if (TimeFromStart + SpawnTimeOffset >=  AConfig[i * ConfigCols] && 
            TimeFromStart + SpawnTimeOffset <=  AConfig[i * ConfigCols] +  AConfig[i * ConfigCols + 1])
            {
                skip = false;
                if (BlockFirstEntry)
                {
                    CurrTimeGap =  AConfig[i * ConfigCols + 2];

                    StartOffset = RangeScale * FVector(AConfig[i * ConfigCols + 3], AConfig[i * ConfigCols + 4], 0.f);
	                EndOffset = RangeScale * FVector(AConfig[i * ConfigCols + 5], AConfig[i * ConfigCols + 6], 0.f);
	                Step = (EndOffset - StartOffset) * (AConfig[i * ConfigCols + 2]/AConfig[i * ConfigCols + 1]);

                    SpinRagne = AConfig[i * ConfigCols + 7];
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
        FVector ypr(UKismetMathLibrary::RandomFloatInRange(-SpinRagne, SpinRagne),
                    UKismetMathLibrary::RandomFloatInRange(-SpinRagne, SpinRagne),
                    UKismetMathLibrary::RandomFloatInRange(-SpinRagne, SpinRagne));
        spin = FRotator(ypr.X,   ypr.Y,   ypr.Z);

        FTransform T = Transform;
        FVector tr = T.GetTranslation();
        tr += StartOffset;
        StartOffset += Step;

        T.SetLocation(tr);
        T.SetRotation(RNormal.Quaternion());
        
        RespawnObject(T, spin);

        TimeFromLast = 0.f;
    }
}