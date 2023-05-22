// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerFlow.h"
#include "FlowObject.h"
#include "FlowModel.h"
#include "Kismet/KismetMathLibrary.h"


void ASpawnerFlow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 
    if (!Enabled) return; if (!Configured) return; if (!Active) return;
    DisactivatePassedObjects();

    TimeFromStart += DeltaTime;
    FRotator rotRoll = UKismetMathLibrary::RotatorFromAxisAndAngle(FVector(0,0,1), TimeFromStart * 180.f/PI * 0.0f);
    this->SetActorRotation(rotRoll.Quaternion());

    TimeFromLast += DeltaTime;
    if (TimeFromLast > SpawnTimeGap)
    {
        FTransform Transform;
        FRotator RNormal(0.f, 0.f, 0.f);

        // for (int i = 0; i < FlowModels.Num(); i++)
        for (int i = 0; i < 1   ; i++)
        {
            // FTransform dT = FlowModels[i]->GetGlobalTransform(0).Inverse() * FlowModels[i]->GetGlobalTransform(SpawnTimeOffset);
            // Transform = Transform * dT.Inverse();

            FTransform T0;
            FTransform TO; // Offset
            FRotator R0;
            FRotator RO; // Offset

            FlowModels[i]->GetTransformAndTilt(0, T0, R0);
            FlowModels[i]->GetTransformAndTilt(SpawnTimeOffset, TO, RO);

            FTransform dT = T0.Inverse() * TO;
            Transform = dT.Inverse();
            RNormal = UKismetMathLibrary::ComposeRotators(RNormal, RO);

        }

        FTransform T = Transform;
        FVector trans = Transform.GetTranslation();
        trans.X += UKismetMathLibrary::RandomFloatInRange(-RandRange.X, RandRange.X);
        trans.Y += UKismetMathLibrary::RandomFloatInRange(-RandRange.Y, RandRange.Y);
        trans.Z += UKismetMathLibrary::RandomFloatInRange(-RandRange.Z, RandRange.Z);
        CurrentRoll += UKismetMathLibrary::RandomFloatInRange(-RollRange, RollRange);
        FRotator roll(0.f, CurrentRoll, 0.f);

        T.SetLocation(trans);

        
        T.SetRotation(UKismetMathLibrary::ComposeRotators(roll, RNormal).Quaternion());
        
        // T.SetRotation(RNormal.Quaternion());

        // if(GEngine && SpawnerID == 6)
	    // {
	    // 	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(
        //         TEXT("SpawnerFlow T %f %f %f %f"), 
        //                 T.GetRotation().X,
        //                 T.GetRotation().Y,
        //                 T.GetRotation().Z,
        //                 T.GetRotation().W
        //         ));	
        // }
         
        RespawnObject(T, FRotator(0.f, 0.f, 0.f));

        // UE_LOG(LogTemp, Warning, TEXT("ASpawner ID %d (SFlow): Spawn Rot %f %f %f %f"), 
        //                      SpawnerID,
        //                      T.Rotator().Quaternion().X,
        //                      T.Rotator().Quaternion().Y,
        //                      T.Rotator().Quaternion().Z,
        //                      T.Rotator().Quaternion().W);

        TimeFromLast = 0.f;
    }
}

