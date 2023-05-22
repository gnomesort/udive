// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowModel.generated.h"

enum EFlowModelType 
{
	FMT_NONE = 0,
	FMT_PWLINEAR,
	FMT_NUM
};

UCLASS()
class UDIVE_API AFlowModel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlowModel();

	// Can't make them abstract because of spawning blueprint inheritors technique
	virtual FTransform GetGlobalTransform(float TimeFromNow, FVector P = FVector());
	virtual FTransform GetIncrementTransform(float TimeFromNow, float DeltaTime, FVector P = FVector());
	virtual void GetTransformAndTilt(float TimeFromNow, FTransform & Transform, FRotator & Rotator, FVector * dR = NULL);
	
	float GetCurrentTime();
	
	UFUNCTION(BlueprintCallable)
	void Restart();

	UFUNCTION(BlueprintCallable)
	void RestartWithSpawners();

	UFUNCTION(BlueprintCallable)
	void Pause();

	UFUNCTION(BlueprintCallable)
	void Resume();

	void AddSpawner(class ASpawner * spawner);

	virtual int SetConfig(const FString& configContent);
	virtual int SetConfig(const TArray<FString>& configLines);

protected:

	EFlowModelType FMType;
	int ConfigCols = 0;
	int ConfigRows = 0;
	TArray<float> AConfig;

	UPROPERTY(EditAnywhere, Category = "FlowModelParameters")
	int FlowModelID = 0;

	bool Configured = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	TArray<class ASpawner*> Spawners;
	float CurrentTime = 0;	

	UPROPERTY(EditAnywhere, Category = "Config")
	class ARoomConfig * RoomConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowModelParameters")
	TSubclassOf<class AFlowObject> AFlowObject_Custom_BP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowModelParameters")
	float SpeedOfTime = 1.f;
	TArray<class AFlowObject*> Helper;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
