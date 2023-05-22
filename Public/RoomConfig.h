// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomConfig.generated.h"

UCLASS()
class UDIVE_API ARoomConfig : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomConfig();

	UPROPERTY(EditAnywhere, Category = "RoomConfigName")
	FString ConfigFileName;

	UFUNCTION(BlueprintCallable)
	void SetConfig(FString fname);

	UFUNCTION(BlueprintCallable)
	FString GetConfig();

	UFUNCTION(BlueprintCallable)
	void RestartRoomSpawners();

	UFUNCTION(BlueprintCallable)
	void StopRoomSpawners();

	void LocateRoomSpawners(FVector center);

	void AddSpawner(class ASpawner * spawner);

	void SetFlowModel(class AFlowModel * fmodel);

	UFUNCTION(BlueprintCallable)
	void SetSpawnersConfig();

	UFUNCTION(BlueprintCallable)
	void SetFlowModelConfig();

protected:

	TArray<class ASpawner*> Spawners;

	// UPROPERTY(EditAnywhere, Category = "FlowModel")
	class AFlowModel* FlowModel;

	virtual void BeginPlay() override;

	
	FString ConfigContent;
	TArray<FString> ConfigLines;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
