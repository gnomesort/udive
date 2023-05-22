// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowObject.generated.h"

UCLASS()
class UDIVE_API AFlowObject : public AActor
{
	GENERATED_BODY()
	
public:	

	AFlowObject();
	void Init(float SpawnTimeArg,	
              TWeakObjectPtr<class AFlowModel> HostFlowArg,	
              TWeakObjectPtr<class ASpawner> HostSpawnerArg);
	void SetHostFlow(TWeakObjectPtr<AFlowModel> HostFlowArg);
	void AddHostFlow(TWeakObjectPtr<AFlowModel> HostFlowArg);
	bool IsActive();
	UFUNCTION(BlueprintCallable)	
	void SetActivity(bool Activity); // just set Active field for activation/disactivation from host spawner
	void Activate(); // includes spawner activation steps
	void Disactivate(); // havy, probably O(Active.size())

	UPROPERTY(EditDefaultsOnly, Category = "SpawnParameters")
	float DefaultScale = 0.2;

	void ApplyTransform(FTransform WorldTransform);
	void SetSpawnTime(float Time);
	void SetLocalTransform(FTransform Transform);
	FTransform GetLocalTransform();
	float GetSpawnTime();
	float GetTimeFromRespawn();

	void SetPermanentSpin(FRotator Rotator);
	void SetStartScale(FVector Scale);
	void SetVisibility(bool visible);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float SpawnTime;
	float RespawnTime;
	float TimeFromRespawn;

	bool Active;
	bool Visible = true;
	FTransform LocalTransform;
	FRotator TotalSpinRotation;
	FRotator SpinRotator;
	FVector StartScale;

	TWeakObjectPtr<class AFlowModel> HostFlow;
	TArray<TWeakObjectPtr<class AFlowModel>> HostFlows;
	TWeakObjectPtr<class ASpawner> HostSpawner;


};
