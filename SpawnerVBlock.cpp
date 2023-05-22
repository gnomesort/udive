// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerVBlock.h"
#include "FlowObject.h"
#include "FlowModel.h"
#include "Kismet/KismetMathLibrary.h"
// #include "Public/LevelConfig.h"

ASpawnerVBlock::ASpawnerVBlock()
{
    SType = ST_VBLOCK;
}

void ASpawnerVBlock::Restart(){
    TimeFromLast = 0;
	TimeFromStart = 0.f;
    BlockFirstEntry = true;
    Active = true;
}

void ASpawnerVBlock::BeginPlay()
{
    Super::BeginPlay();
    // if (SpawnerID == 0)
    // {
    //     Config = &LevelConfig::SPN_VBLCK_0_Config[0];
    //     ConfigCols = LevelConfig::SPN_VBLCK_Col;
    //     ConfigRows = sizeof(LevelConfig::SPN_VBLCK_0_Config)/(sizeof(float) * ConfigCols);
    // } else
    // {
    //     Config = &LevelConfig::SPN_VBLCK_1_Config[0];
    //     ConfigCols = LevelConfig::SPN_VBLCK_Col;
    //     ConfigRows = sizeof(LevelConfig::SPN_VBLCK_1_Config)/(sizeof(float) * ConfigCols);
    // }

    // GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, 
	// 		FString::Printf(TEXT("                          Spawner ID %d , range %f  %f"), 
    //                 SpawnerID, Config[0 * ConfigCols + 11], Config[0 * ConfigCols + 12]));	

}

void ASpawnerVBlock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 

    // if (GEngine)
    // GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Yellow, 
	// 		            FString::Printf(TEXT("                 Spawner ID %d , TimeFromStart %f, T %f  T+DT %f"), 
    //                         SpawnerID, 
    //                         TimeFromStart,
    //                         AConfig[0 * ConfigCols], 
    //                         AConfig[0 * ConfigCols] +  AConfig[0 * ConfigCols + 1]));	
    
    // UE_LOG(LogTemp, Warning, TEXT("                 Spawner ID %d , TimeFromStart %f, T %f  T+DT %f"), 
    //                         SpawnerID, 
    //                         TimeFromStart,
    //                         AConfig[0 * ConfigCols], 
    //                         AConfig[0 * ConfigCols] +  AConfig[0 * ConfigCols + 1]);
    if (!Enabled) return;
    if (!Configured) return; if (!Active) return;
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
                    CurrTimeGap =  AConfig[i * ConfigCols + 2];
                    Scale = FVector(
                        UKismetMathLibrary::RandomFloatInRange(AConfig[i * ConfigCols + 3], AConfig[i * ConfigCols + 6]),
                        UKismetMathLibrary::RandomFloatInRange(AConfig[i * ConfigCols + 4], AConfig[i * ConfigCols + 7]),
                        UKismetMathLibrary::RandomFloatInRange(AConfig[i * ConfigCols + 5], AConfig[i * ConfigCols + 8]));
                    ZeroMargin =  FVector(AConfig[i * ConfigCols + 9], AConfig[i * ConfigCols + 10], 0.f);
                    SpawnRange =  FVector(AConfig[i * ConfigCols + 11], AConfig[i * ConfigCols + 12], 0.f);

                    GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, 
			            FString::Printf(TEXT("                          Spawner ID %d , Zero %f  range %f"), 
                            SpawnerID, ZeroMargin.X, ZeroMargin.Y));	
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
        // for (int i = 0; i < FlowModels.Num(); i++)
        for (int i = 0; i < 1; i++)
        {
            FTransform T0;
            FTransform TO; // Offset
            FRotator R0;
            FRotator RO; // Offset

            FlowModels[i]->GetTransformAndTilt(0, T0, R0);
            FlowModels[i]->GetTransformAndTilt(SpawnTimeOffset, TO, RO);

            FTransform dT = T0.Inverse() * TO;
            // Transform = Transform * dT.Inverse();
            Transform = dT.Inverse();
            RNormal = UKismetMathLibrary::ComposeRotators(RNormal, RO);

            // x

        }

        float ang = UKismetMathLibrary::RandomFloatInRange(0.f, 2.f * PI);
        float rx = UKismetMathLibrary::RandomFloatInRange(ZeroMargin.X, SpawnRange.X);
        float ry = UKismetMathLibrary::RandomFloatInRange(ZeroMargin.Y, SpawnRange.Y);
        FVector offset = FVector(rx * UKismetMathLibrary::Cos(ang),
				                 ry * UKismetMathLibrary::Sin(ang),
				                 0.f);
        FTransform T = Transform;
        FVector tr = T.GetTranslation();
        tr += offset; 

        T.SetLocation(tr);
        T.SetRotation(RNormal.Quaternion());
        T.SetScale3D(Scale);

        RespawnObject(T, FRotator(0.f, 0.f, 0.f));
        // if(GEngine)
	    // {
	    // 	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(
        //         TEXT("SpawnerVBlock T %f %f %f %f"), 
        //                 T.GetRotation().X,
        //                 T.GetRotation().Y,
        //                 T.GetRotation().Z,
        //                 T.GetRotation().W
        //         ));	
        // }

        TimeFromLast = 0.f;
    }
}