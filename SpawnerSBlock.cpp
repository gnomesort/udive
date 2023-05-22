// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerSBlock.h"
#include "FlowObject.h"
#include "FlowModel.h"
#include "Kismet/KismetMathLibrary.h"
// #include "Public/LevelConfig.h"

ASpawnerSBlock::ASpawnerSBlock()
{
    SType = ST_SBLOCK;
}

void ASpawnerSBlock::Restart(){
    TimeFromLast = 0;
	TimeFromStart = 0.f;
    BlockFirstEntry = true;
    Active = true;
}

void ASpawnerSBlock::Tick(float DeltaTime)
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
                    Scale = UKismetMathLibrary::RandomFloatInRange(AConfig[i * ConfigCols + 3], AConfig[i * ConfigCols + 4]);

                    SpinRagne = AConfig[i * ConfigCols + 7];
                    
                    

                    ZeroMargin =  AConfig[i * ConfigCols + 5];
                    SpawnRange =  FVector(AConfig[i * ConfigCols + 6],
                                          AConfig[i * ConfigCols + 6],
                                          AConfig[i * ConfigCols + 6]);
                    
                    // GEngine->AddOnScreenDebugMessage(-1, 100.0, FColor::Blue, FString::Printf(TEXT("shift min %f  max %f"), 
                    //                         ZeroMargin, SpawnRange.X));	
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
        float ang = UKismetMathLibrary::RandomFloatInRange(0.f, 2.f * PI);
        float r = UKismetMathLibrary::RandomFloatInRange(ZeroMargin, SpawnRange.X);
        FVector offset = FVector(r * UKismetMathLibrary::Cos(ang),
				                 r * UKismetMathLibrary::Sin(ang),
				                 0.f);
        FTransform T = Transform;
        FVector tr = T.GetTranslation();

        tr += offset; 

        // if(GEngine)
        // {
        //     GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Green, FString::Printf(TEXT("tr %f %f %f"), tr.X, tr.Y, tr.Z));	
        //     GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Blue, FString::Printf(TEXT("SpawnRange %f %f %f"), 
        //                                                             SpawnRange.X, SpawnRange.Y, SpawnRange.Z));	
        //     // GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Blue, FString::Printf(TEXT("shift min %f  max %f"), 
        //     //                                 ZeroMargin, SpawnRange.X));	
        // }
		    

        T.SetLocation(tr);
        T.SetRotation(RNormal.Quaternion());
        T.SetScale3D(FVector(Scale));
        RespawnObject(T, spin);

        TimeFromLast = 0.f;
    }
}

