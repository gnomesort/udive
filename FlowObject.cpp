// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowObject.h"
#include "FlowModel.h"
#include "Kismet/KismetMathLibrary.h"
#include "Spawner.h"


// Sets default values
AFlowObject::AFlowObject()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    TotalSpinRotation = FRotator(0.f, 0.f, 0.f);
    StartScale = FVector(1.f, 1.f, 1.f);
}

void AFlowObject::SetPermanentSpin(FRotator Rotator)
{
    SpinRotator = Rotator;
}

void AFlowObject::SetStartScale(FVector Scale)
{
    StartScale = Scale;
}

void AFlowObject::Init(float SpawnTimeArg,	
                       TWeakObjectPtr<AFlowModel> HostFlowArg,	
                       TWeakObjectPtr<ASpawner> HostSpawnerArg)
{
    SpawnTime = SpawnTimeArg;
    HostSpawner = HostSpawnerArg;

    TotalSpinRotation = FRotator(0.f,0.f,0.f);

    this->Active = false;
}

void AFlowObject::SetHostFlow(TWeakObjectPtr<AFlowModel> HostFlowArg)
{
    HostFlow = HostFlowArg;
}

void AFlowObject::AddHostFlow(TWeakObjectPtr<AFlowModel> HostFlowArg)
{
    HostFlows.Add(HostFlowArg);
}

bool AFlowObject::IsActive()
{
    
    return Active;
}

void AFlowObject::Activate()
{
    Active = true;
    TotalSpinRotation = FRotator(0.f,0.f,0.f);
    HostSpawner->MoveToActiveArray(this);
}


void AFlowObject::Disactivate()
{
    Active = false;
    HostSpawner->MoveToInactiveArray(this);
}

// Called when the game starts or when spawned
void AFlowObject::BeginPlay()
{
    Super::BeginPlay();
}

void AFlowObject::SetLocalTransform(FTransform Transform)
{
    LocalTransform = Transform;
}

FTransform AFlowObject::GetLocalTransform()
{
    return LocalTransform;
}

// Called every frame
void AFlowObject::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // TimeFromRespawn += DeltaTime;
    float TimePrev = TimeFromRespawn;
    if (HostFlows.Num() > 0)
        TimeFromRespawn = HostFlows[0]->GetCurrentTime() - RespawnTime;

    // GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Red, 
	// 			FString::Printf(TEXT("HostFlows.Num()  %d"), 
	// 						HostFlows.Num()));	

    // if(GEngine && HostFlows.Num() > 0){
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Red, 
	// 			FString::Printf(TEXT("dt1 = %f, dt2 = %f"), 
	// 						TimeFromRespawn, HostFlows[0]->GetCurrentTime() - RespawnTime));	
	// }

    if (Active)
    {
        for (int i = 0; i < HostFlows.Num(); i++)
        {
            if (!HostFlows[i].IsValid())
            {
                UE_LOG(LogTemp, Error, TEXT("HostFlow is NOT valid"));
                return;
            }
    
            // LocalTransform = LocalTransform * HostFlows[i]->GetIncrementTransform(0, DeltaTime);
            LocalTransform = LocalTransform * HostFlows[i]->GetIncrementTransform(0, TimeFromRespawn - TimePrev);
        }
        this->SetActorTransform(LocalTransform * HostSpawner->GetSpawnerBaseTransform());
        this->SetActorScale3D(this->StartScale);

        TotalSpinRotation = UKismetMathLibrary::ComposeRotators(TotalSpinRotation, SpinRotator);
        this->SetActorRotation( UKismetMathLibrary::ComposeRotators(TotalSpinRotation, this->GetActorRotation()).Quaternion());

    }
}

void AFlowObject::ApplyTransform(FTransform WorldTransform)
{

}
void AFlowObject::SetSpawnTime(float Time)
{
    SpawnTime = Time;
}
float AFlowObject::GetSpawnTime()
{
    return SpawnTime;
}

float AFlowObject::GetTimeFromRespawn()
{
    return TimeFromRespawn;
}

void AFlowObject::SetActivity(bool Activity)
{
    if (Active == false){
        if (HostFlows.Num() > 0)
            RespawnTime = HostFlows[0]->GetCurrentTime();
        TimeFromRespawn = 0.f;
    }
        
    Active = Activity;
    SetActorHiddenInGame(!Activity);    
}

void AFlowObject::SetVisibility(bool visible)
{
    Visible = visible;
    // if(GEngine){
	// 	GEngine->AddOnScreenDebugMessage(-1, 10.5, FColor::Red, 
	// 			// FString::Printf(TEXT("Visible = %d"), (int)Visible));	
    //             FString::Printf(TEXT("Active = %d  Visible = %d"), (int)Active, (int)Visible));	
	// }

    // SetActorHiddenInGame(!Active && !Visible);
    SetActorHiddenInGame(!(Visible && Visible));
}

