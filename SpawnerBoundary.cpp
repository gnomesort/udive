// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerBoundary.h"
#include "FlowObject.h"
#include "FlowModel.h"
#include "Kismet/KismetMathLibrary.h"

ASpawnerBoundary::ASpawnerBoundary()
{
}

void ASpawnerBoundary::SetGuardianPoints(TArray<FVector> Points)
{
    if (Points.Num() > 0 && !SimBoundary)
    {
        for (int i = 0; i < Points.Num(); i++)
        {
            // GuardianPoints.Add(Points[i]);
            // GuardianPoints.Add(
            //     FVector(Points[i].X, 0.f, Points[i].Y));

            GuardianPoints.Add(
                FVector(Points[i].X, Points[i].Y, 0.f));
        }
            
    } else 
    {
        
        for (int i = 0; i < SimBndNum; i++)
        {
            FVector V(SimBndRadius * (UKismetMathLibrary::Sin(2 * PI * (float)i/(float)SimBndNum)), 
                      SimBndRadius * (UKismetMathLibrary::Cos(2 * PI * (float)i/(float)SimBndNum)),
                      0.f);

            GuardianPoints.Add(V);
        }
    }

    if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("pts num %d"),GuardianPoints.Num()));	
    }

}



void ASpawnerBoundary::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 
    if (!Enabled) return; if (!Configured) return; if (!Active) return;
    DisactivatePassedObjects();

    static float TimeFromStart = 0.f;
    TimeFromStart += DeltaTime;

    static float TimeFromLast = 0;
    static int mod = 0;
    TimeFromLast += DeltaTime;

    if (TimeFromLast > SpawnTimeGap)
    {
        // if(GEngine)
    	// 	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("spawn new")));	

        FTransform Transform;
        FRotator Rotator;
        for (int i = 0; i < FlowModels.Num(); i++)
        {
            FTransform T0;
            FTransform TO; // Offset
            FRotator R0;
            FRotator RO; // Offset

            FlowModels[i]->GetTransformAndTilt(0, T0, R0);
            FlowModels[i]->GetTransformAndTilt(SpawnTimeOffset, TO, RO);


            // FTransform dT = FlowModels[i]->GetGlobalTransform(0).Inverse() * FlowModels[i]->GetGlobalTransform(SpawnTimeOffset);
            FTransform dT = T0.Inverse() * TO;
            Transform = Transform * dT.Inverse();
            Rotator = Rotator + RO;
        }

        // FVector trans = Transform.GetTranslation();
        // Transform.SetLocation(trans);
        // RespawnObject(Transform);
        int iratio = GuardianPoints.Num()/BndSpawnNum;
        for (int i = 0; i < GuardianPoints.Num(); i++)
        {
            if ((i % iratio) == mod)
            {
                FTransform T = Transform;
                FVector tr = T.GetTranslation();
                tr.X += GuardianPoints[i].X;
                tr.Y += GuardianPoints[i].Y;
                tr.Z += GuardianPoints[i].Z;
                T.SetLocation(tr);
                // T.SetRotation(Rotator.Quaternion());
                RespawnObject(T, FRotator(0.f, 0.f, 0.f));
            }
        }
        mod = (mod + SimBndShift) % iratio;
        
        
        TimeFromLast = 0.f;
    }
}