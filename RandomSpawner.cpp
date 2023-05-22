// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomSpawner.h"
#include "FlowObject.h"
#include "Kismet/KismetMathLibrary.h"


bool ARandomSpawner::TimeToDisactive(TWeakObjectPtr<class AFlowObject> FObject)
{
    
    if (FObject->GetLocalTransform().GetTranslation().Length() > DisactivateDistance ||
        FObject->GetLocalTransform().GetScale3D().Length() > DisactivateScale)
        return true;
    else
        return false;
}

void ARandomSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!Enabled) return; if (!Configured) return; if (!Active) return;
    DisactivatePassedObjects();

    static float TimeFromLast = 0;
    TimeFromLast += DeltaTime;
    if (TimeFromLast > SpawnTimeGap)
    {
        FTransform transform;
        transform.SetTranslation(
            UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(ZeroMargine, SpawnRange));
        RespawnObject(transform);
        TimeFromLast = 0.f;
    }
}

// bool ARandomSpawner::RespawnObject(FTransform Transform)
// {
//     // Super::RespawnObject(Transform);
//     UE_LOG(LogTemp, Warning, TEXT("ARandomSpawner::RespawnObject"));

//     if (InactiveObjects.Num() == 0)
//     {
//         UE_LOG(LogTemp, Warning, TEXT("RespawnObject: There is no free object"));
//         return false;
//     }

//     TWeakObjectPtr<AFlowObject> ObjectToSpawn = InactiveObjects[0];
//     MoveToActiveArray(0);
    
//     FTransform ObjectTransform;
//     ObjectTransform = Transform;
//     ObjectTransform.SetScale3D(FVector(ObjectToSpawn->DefaultScale));
//     ObjectToSpawn->SetLocalTransform(ObjectTransform);
//     ObjectToSpawn->SetActivity(true);

//     return true;
// }