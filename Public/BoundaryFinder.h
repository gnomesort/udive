// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoundaryFinder.generated.h"

UCLASS()
class UDIVE_API ABoundaryFinder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoundaryFinder();

	
	UPROPERTY(BlueprintReadWrite, Category = "CircleMesh")
	class UProceduralMeshComponent * PCircleMesh;

	UPROPERTY(EditAnywhere, Category = "CircleMesh")
	int Perimeter = 100;

	UPROPERTY(EditAnywhere, Category = "CircleMesh")
	float Altitude = 0.f;

	
	UPROPERTY(BlueprintReadWrite, Category = "TrajectoryMesh")
	class UProceduralMeshComponent * PTrajectoryMesh;

	UPROPERTY(EditAnywhere, Category = "TrajectoryMesh")
	float NodeSize = 20.f;

	UPROPERTY(EditAnywhere, Category = "TrajectoryMesh")
	int TrajectoryNodsLimit = 50;

	UPROPERTY(EditAnywhere, Category = "TrajectoryMesh")
	int Density = 40;

	UFUNCTION(BlueprintCallable)
	void AddTrajectoryNode(FVector p);

	UFUNCTION(BlueprintCallable)
	void Update(FVector head);

	UFUNCTION(BlueprintCallable)
	void EsimateCircle(FVector NewNode, FVector& Center, float& Radius);

	UFUNCTION(BlueprintCallable)
	void GetCircle(FVector& Center, float& Radius);

	UFUNCTION(BlueprintCallable)
	void SetDefaultCenter(FVector Center);

	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION(BlueprintCallable)
	void Stop();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Defaults")
	float DefaultRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "Defaults")
	float MaxRadius = 215.f;

	UPROPERTY(EditAnywhere, Category = "Defaults")
	FVector DefaultCenter = FVector(0.f, 0.f, 0.f);

protected:

	bool Active = true;
	bool UpdateCenter = true;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool GetCircle3P(const FVector2D& A, const FVector2D& B, const FVector2D& C, 
						FVector2D& Center, float& Radius);

	void UpdateCircleMesh(FVector Center, float Radius);

	FVector PreviousNodePosition;
	float CurrentRadius = 300.f;
	FVector CurrentCenter = FVector(0.f, 0.f, 0.f);

	TArray<FVector> TrajectoryNodes;

	TArray<FVector> TNodeMeshVertices;
	TArray<int> TNodeMeshTriangles;
	
	TArray<FVector> TrajectoryMeshVertices;
	TArray<int> TrajectoryMeshTriangles;

	TArray<FVector> CircleMeshVertices;
	TArray<int> CircleMeshTriangles;
};