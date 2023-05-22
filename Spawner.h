// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <memory>
#include "Spawner.generated.h"


enum ESpawnerType 
{
	ST_NONE = 0,
	ST_RING,
	ST_COINS,
	ST_GATE,
	ST_MBLOCK,
	ST_VBLOCK,
	ST_SBLOCK,
	ST_FLOW,
	ST_NUM
};

UCLASS()
class UDIVE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	bool Enabled = false;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	bool Active = false;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	bool PlaceToZero = true;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	int SpawnerID = 0;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	float SpawnTimeOffset = 3.f;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	int ObjectsAllocatedNum = 10;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	bool SpawnerLocationOffset = true;

	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	bool LogConfig = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnParameters")
	TSubclassOf<class AFlowObject> AFlowObject_Custom_BP;
	
	UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	TArray<class AFlowModel*> FlowModels;

	UPROPERTY(EditAnywhere, Category = "Config")
	class ARoomConfig * RoomConfig;

	UFUNCTION(BlueprintCallable)
	class ARoomConfig * GetRoomConfig();

	bool MoveToActiveArray(TWeakObjectPtr<class AFlowObject> FObject);
	bool MoveToInactiveArray(TWeakObjectPtr<class AFlowObject> FObject);
	bool MoveToActiveArray(int index);
	bool MoveToInactiveArray(int index);

	UFUNCTION(BlueprintCallable)
	virtual void Restart();

	UFUNCTION(BlueprintCallable)
	void Stop();

	FTransform GetSpawnerBaseTransform();

	int SetConfig(const FString& configContent);

	UFUNCTION(BlueprintCallable)
	int SetConfig();

	UFUNCTION(BlueprintCallable)
	int GetID();

	int GetConfigCols();

protected:

	ESpawnerType SType;

	int ConfigCols = 0;
	int ConfigRows = 0;
	TArray<float> AConfig;
	// const float * Config;
	bool Visible = true;
	bool Configured = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool TimeToDisactive(TWeakObjectPtr<class AFlowObject> FObject);

	virtual bool RespawnObject(FTransform Transform, FRotator Spin = FRotator());

	void DisactivatePassedObjects();

	UFUNCTION(BlueprintCallable)
	void SetFlows();

	UFUNCTION(BlueprintCallable)
	void InitStorage();

	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool visible);

	UFUNCTION(BlueprintCallable)
	void SetEnable(bool enable);

	TArray<TWeakObjectPtr<class AFlowObject>> InactiveObjects;
	TArray<TWeakObjectPtr<class AFlowObject>> ActiveObjects;

	TWeakObjectPtr<class AFlowObject> GetInactive();

	// UPROPERTY(EditAnywhere, Category = "SpawnParameters")
	// FVector AdditionalScale(1.f);
};
