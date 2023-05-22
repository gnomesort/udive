// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerLooped.h"
#include "FlowObject.h"
#include "Kismet/KismetMathLibrary.h"

void ASpawnerLooped::BeginPlay()
{
    Super::BeginPlay();
    for (int i = 0; i < ObjectsAllocatedNum; i++)
    {
        FTransform Transform;
        FVector Translation(UKismetMathLibrary::RandomFloatInRange(-SpawningBox.X, SpawningBox.X),
                            UKismetMathLibrary::RandomFloatInRange(-SpawningBox.Y, SpawningBox.Y),
                            UKismetMathLibrary::RandomFloatInRange(-SpawningBox.Z, SpawningBox.Z));
        Transform.SetTranslation(Translation);
        RespawnObject(Transform);
    }

}

void ASpawnerLooped::Tick(float DeltaTime)
{
    if (!Enabled) return; if (!Configured) return; if (!Active) return;
    
    Super::Tick(DeltaTime); 
    for (auto ptr: ActiveObjects)
    {
        FTransform LocalTransform = ptr->GetLocalTransform();
        FVector LocalTranslation = ptr->GetLocalTransform().GetTranslation();
        if (LocalTranslation.X > SpawningBox.X) {
            LocalTranslation.X -= 2 * SpawningBox.X; }
        if (LocalTranslation.X < -SpawningBox.X) {
            LocalTranslation.X += 2 * SpawningBox.X; }
        if (LocalTranslation.Y > SpawningBox.Y) {
            LocalTranslation.Y -= 2 * SpawningBox.Y; }
        if (LocalTranslation.Y < -SpawningBox.Y) {
            LocalTranslation.Y += 2 * SpawningBox.Y; }
        if (LocalTranslation.Z > SpawningBox.Z) {
            LocalTranslation.Z -= 2 * SpawningBox.Z; }
        if (LocalTranslation.Z < -SpawningBox.Z) {
            LocalTranslation.Z += 2 * SpawningBox.Z; }
        LocalTransform.SetTranslation(LocalTranslation);
        ptr->SetLocalTransform(LocalTransform);
    }
}