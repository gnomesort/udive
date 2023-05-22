// Fill out your copyright notice in the Description page of Project Settings.
#include "SpawnerMBlock.h"
#include "FlowObject.h"
#include "FlowModel.h"
#include "Kismet/KismetMathLibrary.h"
// #include "Public/LevelConfig.h"

ASpawnerMBlock::ASpawnerMBlock()
{
    SType = ST_MBLOCK;
}

void ASpawnerMBlock::Restart(){
    TimeFromLast = 0;
	TimeFromStart = 0.f;
    BlockFirstEntry = true;
    Active = true;
}


void ASpawnerMBlock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 
    if (!Enabled) return; if (!Configured) return; if (!Active) return;
    DisactivatePassedObjects();
    
    TimeFromStart += DeltaTime;
    TimeFromLast += DeltaTime;
    
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
                    CurrTimeGap =  AConfig[i * ConfigCols + 2];
                    rangeRoll =  AConfig[i * ConfigCols + 3];
                    CurrDevRange =  AConfig[i * ConfigCols + 4];
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
        FVector DR(0.f, 0.f, 0.f);
        for (int i = 0; i < FlowModels.Num(); i++)
        {
            FTransform T0;
            FTransform TO; // Offset
            FRotator R0;
            FRotator RO; // Offset

            FlowModels[i]->GetTransformAndTilt(0, T0, R0);
            FlowModels[i]->GetTransformAndTilt(SpawnTimeOffset, TO, RO, &DR);

            FTransform dT = T0.Inverse() * TO;
            Transform = Transform * dT.Inverse();
            RNormal = UKismetMathLibrary::ComposeRotators(RNormal, RO);
        }

        FRotator rotA(
            UKismetMathLibrary::RandomFloatInRange(-CurrDevRange, CurrDevRange),
            UKismetMathLibrary::RandomFloatInRange(-CurrDevRange, CurrDevRange),
            UKismetMathLibrary::RandomFloatInRange(-CurrDevRange, CurrDevRange)); 

        float offsetRoll = -90.f;
        DR.Z = 0.f;
        FVector fwd(0.f, 1.f, 0.f);
        if (DR.Length() > 1e-5)
        {
            float cos = FVector::DotProduct(fwd, DR)/(DR.Length() * fwd.Length());
	        offsetRoll -= UKismetMathLibrary::Acos(cos) * 180.f / PI;
        }     

        // float currentRoll = switcher == 0 ? offsetRoll - rangeRoll : offsetRoll + rangeRoll;
        float currentRoll = switcher == 0 ? offsetRoll - 30.f : offsetRoll + 30.f;

        FTransform T = Transform;
        FVector tr = T.GetTranslation();
        T.SetLocation(tr);
        FRotator roll(0.f, currentRoll, 0.f);
        FRotator fin = UKismetMathLibrary::ComposeRotators(roll, rotA); 
        T.SetRotation(UKismetMathLibrary::ComposeRotators(fin, RNormal).Quaternion());
        RespawnObject(T, FRotator(0.f, 0.f, 0.f));

        UE_LOG(LogTemp, Warning, TEXT("ASpawner ID %d (Block): Spawn Rot %f %f %f %f"), 
                             SpawnerID,
                             T.Rotator().Quaternion().X,
                             T.Rotator().Quaternion().Y,
                             T.Rotator().Quaternion().Z,
                             T.Rotator().Quaternion().W);
        

        TimeFromLast = 0.f;
        switcher = 1 - switcher;
    }
}
