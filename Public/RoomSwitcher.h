// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomSwitcher.generated.h"

enum ERoomId
{
	ROOM_START = 0,
	ROOM_V1,
	ROOM_V2,
	ROOM_H1,
	ROOM_H2,
	ROOM_MM,
	ROOM_FF,
	ROOM_NUM
};

enum EGridId
{
	GRID_0 = 0,
	GRID_1,
	GRID_2,
	GRID_NUM
};

enum ESessionState
{
	SST_BOUNDARY = 0,
	SST_ROOM_CHOOSE,
	SST_FLIGHT,
	SST_NUM
};



UCLASS()
class UDIVE_API ARoomSwitcher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomSwitcher();

	/* Floor */
	UPROPERTY(BlueprintReadWrite, Category = "Floor")
	class UStaticMeshComponent* MC_FloorLogo;
	UPROPERTY(BlueprintReadWrite, Category = "Floor")
	class UProceduralMeshComponent* MC_FloorHex;
	UPROPERTY(BlueprintReadWrite, Category = "Floor")
	class UProceduralMeshComponent* MC_FloorNodes;

	/* Grid */
	UPROPERTY(BlueprintReadOnly, Category = "Grid")
	class UProceduralMeshComponent* MC_GridRods;
	UPROPERTY(BlueprintReadOnly, Category = "Grid")
	class UProceduralMeshComponent* MC_GridCells;
	UPROPERTY(BlueprintReadOnly, Category = "Grid")
	class UProceduralMeshComponent* MC_GridDots;
	
	/* Elements */
	UPROPERTY(BlueprintReadOnly, Category = "Elements")
	class UProceduralMeshComponent* MC_CenterCrossbar;
	UPROPERTY(BlueprintReadOnly, Category = "Elements")
	class UProceduralMeshComponent* MC_LeftCrossbar;
	UPROPERTY(BlueprintReadOnly, Category = "Elements")
	class UProceduralMeshComponent* MC_RightCrossbar;
	UPROPERTY(BlueprintReadOnly, Category = "Elements")
	class UProceduralMeshComponent* MC_BottomCrossbar;
	UPROPERTY(BlueprintReadOnly, Category = "Elements")
	class UProceduralMeshComponent* MC_BoundaryRing;
	UPROPERTY(BlueprintReadOnly, Category = "Elements")
	class UProceduralMeshComponent* MC_TopCrossbar;

	/* Room Configs */
	UPROPERTY(EditAnywhere, Category = "RoomConfigs")
	class ARoomConfig * FreeFallRoomConfig;
	UPROPERTY(EditAnywhere, Category = "RoomConfigs")
	class ARoomConfig * MarioModeRoomConfig;

	/* Boundary Finder */
	UPROPERTY(EditAnywhere, Category = "BoundaryFinder")
	class ABoundaryFinder * BoundaryFinder;

	/* Main Function */	
	UFUNCTION(BlueprintCallable)
	void UpdateElements(FVector Center, float Radius);
	UFUNCTION(BlueprintCallable)
	int ChangeState();
	UFUNCTION(BlueprintCallable)
	void ChangeEnvironment();

	/* Get Material Parameters */
	UFUNCTION(BlueprintCallable)
	FVector GetGridColor();
	UFUNCTION(BlueprintCallable)
	FVector GetFloorColor();
	UFUNCTION(BlueprintCallable)
	float GetGridTransparency();
	UFUNCTION(BlueprintCallable)
	float GetFloorTransparency();
	UFUNCTION(BlueprintCallable)
	float GetGridGlow();
	UFUNCTION(BlueprintCallable)
	float GetFloorGlow();

	UFUNCTION(BlueprintCallable)
	void SwitchRoom();

protected:

	UPROPERTY(EditAnywhere, Category = "BoundaryFinder")
	float DefaultRadius = 1000.f; // shouldn't be used, use roomConfig defaults instead
	UPROPERTY(EditAnywhere, Category = "BoundaryFinder")
	FVector DefaultCenter = FVector(0.f, 0.f, 0.f);

	/* Material Parameters */
	FVector GridColor = FVector(1.f, 1.f, 1.f);
	FVector FloorColor= FVector(1.f, 1.f, 0.3f);
	float GridTransparency = 0.5;
	float FloorTransparency = 0.5;
	float GridGlow = 10.f;
	float FloorGlow = 10.f;

	void UpdateGrid(int GridId, FVector Center, float Radius);
	void UpdateGridDots(FVector Center, float Radius);
	void UpdateGridRods(FVector Center, float Radius);
	void UpdateGridCells(FVector Center, float Radius);
	void UpdateFloorNodes(FVector Center, float Radius);
	void UpdateFloorHex(FVector Center, float Radius);

	void UpdateCrossBars(FVector Center, float Radius);
	void UpdateCrossVBars(FVector Center, float Radius);
	void UpdateCrossHBars(FVector Center, float Radius);
	void UpdateBoundaryRing(FVector Center, float Radius);

	
	void BuildGridNodes();
	void BuildFloorNodes();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int CurrentRoom = (int)ROOM_NUM - 1;
	int CurrentState = (int)SST_BOUNDARY;

	TArray<class UProceduralMeshComponent*> Elements;

	/* Elemets */

	UPROPERTY(EditAnywhere, Category = "Emelemts")
	float RingH = 120;
	UPROPERTY(EditAnywhere, Category = "Emelemts")
	float RingW = 20;
	UPROPERTY(EditAnywhere, Category = "Emelemts")
	float SideCrossbarShift = 0.5; // 0 - Center, 1 - Boundary
	UPROPERTY(EditAnywhere, Category = "Emelemts")
	float SideCrossbarHeight = 500;
	UPROPERTY(EditAnywhere, Category = "Emelemts")
	float BottomCrossbarHeight = 70;
	UPROPERTY(EditAnywhere, Category = "Emelemts")
	float TopCrossbarHeight = 200;
	UPROPERTY(EditAnywhere, Category = "Emelemts")
	float CrossbarDiam = 15;


	/* Grid Common */
	UPROPERTY(EditAnywhere, Category = "Grid Common")
	float LowerBoundary = 0.1;
	UPROPERTY(EditAnywhere, Category = "Grid Common")
	float UpperBoundary = 400.f;
	
	/* Dot Grid */
	UPROPERTY(EditAnywhere, Category = "Grid Dots")
	int GridDots_Perimeter = 100;
	UPROPERTY(EditAnywhere, Category = "Grid Dots")
	float GridDots_NodeSize = 5.f;
	UPROPERTY(EditAnywhere, Category = "Grid Dots")
	float GridDots_Screw = 0.5;
	UPROPERTY(EditAnywhere, Category = "Grid Dots")
	float GridDots_Transparency = 0.9;

	TArray<FVector> GridDots_NodeVertices;
	TArray<int> GridDots_NodeTriangles;

	/* Rod Grid */
	UPROPERTY(EditAnywhere, Category = "Grid Rods")
	int GridRods_Perimeter = 100;
	UPROPERTY(EditAnywhere, Category = "Grid Rods")
	float GridRods_Transparency = 0.9;

	/* Cell Grid */
	UPROPERTY(EditAnywhere, Category = "Grid Cells")
	int GridCells_Perimeter = 100;
	UPROPERTY(EditAnywhere, Category = "Grid Cells")
	float GridCells_OuterR = 0.9; // related to half cell size 
	UPROPERTY(EditAnywhere, Category = "Grid Cells")
	float GridCells_InnerR = 0.1; // related to half cell size 
	UPROPERTY(EditAnywhere, Category = "Grid Cells")
	float GridCells_Screw = 0.5;

	TArray<FVector> GridCells_NodeVertices;
	TArray<int> GridCells_NodeTriangles;


	/* Floor Common */
	UPROPERTY(EditAnywhere, Category = "Floor Common")
	float hOffset = 0.f;

	/* Hex Floor */
	UPROPERTY(EditAnywhere, Category = "Floor Hex")
	float HexTileSize = 60.f;
	UPROPERTY(EditAnywhere, Category = "Floor Hex")
	float HexTileGap = 10.f;
	UPROPERTY(EditAnywhere, Category = "Floor Hex")
	float HexFloorRadius = 1.3f; // related to tube radius

	/* Nodes Floor */
	UPROPERTY(EditAnywhere, Category = "Floor Nodes")
	float FloorNodes_CellSize = 30.f;
	UPROPERTY(EditAnywhere, Category = "Floor Nodes")
	float FloorNodes_Radius = 1.3f; // related to tube radius
	UPROPERTY(EditAnywhere, Category = "Floor Nodes")
	float FloorNodes_OuterR = 0.9; // related to half cell size 
	UPROPERTY(EditAnywhere, Category = "Floor Nodes")
	float FloorNodes_InnerR = 0.1; // related to half cell size 

	TArray<FVector> FloorNodes_NodeVertices;
	TArray<int> FloorNodes_NodeTriangles;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
