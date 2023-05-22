// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../FlowObject.h"
#include "FlowObjectGG.generated.h"

/**
 * 
 */
UCLASS()
class UDIVE_API AFlowObjectGG : public AFlowObject
{
	GENERATED_BODY()

	AFlowObjectGG();
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, Category = "Grid")
	class UProceduralMeshComponent * PMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Floor")
	class UProceduralMeshComponent * FloorMesh;

	UFUNCTION(BlueprintCallable)
	void BuildGuardianGrid(TArray<FVector> Points);
	TArray<FVector> Vertices; 
	TArray<int> Triangles;
	TArray<FVector2D> UV0;

	TArray<FVector> FloorVertices; 
	TArray<int> FloorTriangles;

	float TimeFromStart = 0.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float HeightTop = 300.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float HeightBottom = -40.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float SensDistMin = 1.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float SensDistMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float SensAltitudeRatio = 2.f;

	UPROPERTY(EditAnywhere, Category = "Floor")
	float HexTileSize = 60.f;

	UPROPERTY(EditAnywhere, Category = "Floor")
	float HexTileGap = 10.f;

	UPROPERTY(EditAnywhere, Category = "Floor")
	float FloorRadius = 1000.f;
};
