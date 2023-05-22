// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GuardGridGenerator.generated.h"

UCLASS()
class UDIVE_API AGuardGridGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuardGridGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void BuildGuardianGrid(TArray<FVector> Points);

	// UFUNCTION(BlueprintCallable)
	// class UMaterialInterface * GetCustomMat();

	UFUNCTION(BlueprintCallable)
	float GetTransparency(FTransform Transform, float & dist);

	UPROPERTY(BlueprintReadWrite, Category = "Grid")
	class UProceduralMeshComponent * PMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Floor")
	class UProceduralMeshComponent * FloorMesh;

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

	UPROPERTY(EditAnywhere, Category = "Grid")
	float MaxVisibilityDuration = 7.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float DisappearingDuration = 2.f;


	// UPROPERTY(EditAnywhere, Category = "Grid")
	// class UMaterialInterface * CustomMat;

	UPROPERTY(EditAnywhere, Category = "Floor")
	float HexTileSize = 60.f;

	UPROPERTY(EditAnywhere, Category = "Floor")
	float HexTileGap = 10.f;

	UPROPERTY(EditAnywhere, Category = "Floor")
	float FloorRadius = 1000.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
