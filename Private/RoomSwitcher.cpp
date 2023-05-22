// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSwitcher.h"
#include "GameFramework/Actor.h"
// #include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "RoomConfig.h"
#include "BoundaryFinder.h"

#include "../Spawner.h"
#include "../FlowModel.h"


// Sets default values
ARoomSwitcher::ARoomSwitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MC_FloorLogo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorLogo"));
	MC_FloorLogo->SetupAttachment(GetRootComponent());

	MC_FloorHex = CreateDefaultSubobject<UProceduralMeshComponent>("FloorHex");		Elements.Add(MC_FloorHex);
	MC_FloorNodes = CreateDefaultSubobject<UProceduralMeshComponent>("FloorNodes");	Elements.Add(MC_FloorNodes);
	MC_GridRods = CreateDefaultSubobject<UProceduralMeshComponent>("GridRods");		Elements.Add(MC_GridRods);
	MC_GridCells = CreateDefaultSubobject<UProceduralMeshComponent>("GridCells");	Elements.Add(MC_GridCells);
	MC_GridDots = CreateDefaultSubobject<UProceduralMeshComponent>("GridDots");		Elements.Add(MC_GridDots);
	MC_CenterCrossbar = CreateDefaultSubobject<UProceduralMeshComponent>("CenterCrossbar");	Elements.Add(MC_CenterCrossbar);
	MC_LeftCrossbar = CreateDefaultSubobject<UProceduralMeshComponent>("LeftCrossbar");		Elements.Add(MC_LeftCrossbar);
	MC_RightCrossbar = CreateDefaultSubobject<UProceduralMeshComponent>("RightCrossbar");	Elements.Add(MC_RightCrossbar);
	MC_BottomCrossbar = CreateDefaultSubobject<UProceduralMeshComponent>("BottomCrossbar");	Elements.Add(MC_BottomCrossbar);
	MC_TopCrossbar = CreateDefaultSubobject<UProceduralMeshComponent>("TopCrossbar");		Elements.Add(MC_TopCrossbar);
	MC_BoundaryRing = CreateDefaultSubobject<UProceduralMeshComponent>("BoundaryRing");		Elements.Add(MC_BoundaryRing);

	for (auto element : Elements)
	{
		element->SetupAttachment(GetRootComponent());
		// element->SetWorldLocation(FVector(0));
	}
}

FVector ARoomSwitcher::GetGridColor(){
	return GridColor;
}

FVector ARoomSwitcher::GetFloorColor(){
	return FloorColor;
}

float ARoomSwitcher::GetGridTransparency(){
	return GridTransparency;
}

float ARoomSwitcher::GetFloorTransparency(){
	return FloorTransparency;
}

float ARoomSwitcher::GetGridGlow(){
	return GridGlow;
}
float ARoomSwitcher::GetFloorGlow(){
	return FloorGlow;
}


void ARoomSwitcher::SwitchRoom()
{
	

	float radius = DefaultRadius;
	FVector center = DefaultCenter;

	if (BoundaryFinder)
		BoundaryFinder->GetCircle(center, radius);

	for (auto element : Elements)
		element->SetHiddenInGame(true);

	if (MarioModeRoomConfig){
		MarioModeRoomConfig->StopRoomSpawners();
		MarioModeRoomConfig->LocateRoomSpawners(center);
	} else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		"								MarioModeRoomConfig NULL");

	if (FreeFallRoomConfig){
		FreeFallRoomConfig->LocateRoomSpawners(center);
		FreeFallRoomConfig->StopRoomSpawners();
	} else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		"								FreeFallRoomConfig NULL");

	CurrentRoom = (CurrentRoom + 1) % (int)ROOM_NUM;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
                            FString::Printf(TEXT(" Switch Room to %d "), CurrentRoom));	
	
	switch (CurrentRoom)
	{
	case (int)ROOM_START:
		FloorColor = FVector(1.f, 1.f, 0.3f);
		FloorTransparency = 0.5;
		FloorGlow = 10.f;

		HexTileSize = 60.f;
		HexTileGap = 10.f;
		HexFloorRadius = 2.0f;

		UpdateFloorHex(DefaultCenter, DefaultRadius);
		MC_FloorHex->SetHiddenInGame(false);

		break;

	case (int)ROOM_V1:
		GridColor = FVector(1.f, 0.f, 1.f);
		FloorColor = FVector(0.5f, 1.0f, 0.3f);
		GridTransparency = 0.5;
		FloorTransparency = 0.5;
		GridGlow = 10.f;
		FloorGlow = 10.f;

		GridCells_Perimeter = 30;
		GridCells_OuterR = 0.8;
		GridCells_InnerR = 0.1;
		GridCells_Screw = 0.0;

		FloorNodes_CellSize = 15.f;
		FloorNodes_Radius = 1.3f;
		FloorNodes_OuterR = 0.9;
		FloorNodes_InnerR = 0.1;

		BuildFloorNodes();
		BuildGridNodes();

		UpdateGridCells(center, radius);
		UpdateFloorNodes(center, radius);
		UpdateCrossVBars(center, radius);

		MC_GridCells->SetHiddenInGame(false);
		MC_FloorNodes->SetHiddenInGame(false);
		MC_CenterCrossbar->SetHiddenInGame(false);

		break;

	case (int)ROOM_V2:
		GridColor = FVector(0.2f, 0.f, 1.f);
		FloorColor = FVector(1.2f, 0.f, 0.3f);
		GridTransparency = 0.5;
		FloorTransparency = 0.5;
		GridGlow = 10.f;
		FloorGlow = 10.f;

		GridCells_Perimeter = 30;
		GridCells_OuterR = 0.1;
		GridCells_InnerR = 0.9;
		GridCells_Screw = 0.05;

		FloorNodes_CellSize = 25.f;
		FloorNodes_Radius = 1.0f;
		FloorNodes_OuterR = 0.45;
		FloorNodes_InnerR = 0.25;

		BuildFloorNodes();
		BuildGridNodes();

		UpdateGridCells(center, radius);
		UpdateFloorNodes(center, radius);
		UpdateCrossVBars(center, radius);

		MC_GridCells->SetHiddenInGame(false);
		MC_FloorNodes->SetHiddenInGame(false);
		MC_LeftCrossbar->SetHiddenInGame(false);
		MC_RightCrossbar->SetHiddenInGame(false);

		break;

	case (int)ROOM_H1:
		GridColor = FVector(0.1f, 0.9f, 1.f);
		FloorColor = FVector(0.2f, 0.f, 0.3f);
		GridTransparency = 0.5;
		FloorTransparency = 0.5;
		GridGlow = 10.f;
		FloorGlow = 10.f;

		GridCells_Perimeter = 20;
		GridCells_OuterR = 0.9;
		GridCells_InnerR = 0.1;
		GridCells_Screw = 0.1;

		FloorNodes_CellSize = 30.f;
		FloorNodes_Radius = 1.0f;
		FloorNodes_OuterR = 0.8;
		FloorNodes_InnerR = 0.1;

		BuildFloorNodes();
		BuildGridNodes();

		UpdateGridCells(center, radius);
		UpdateFloorNodes(center, radius);
		UpdateCrossHBars(center, radius);

		MC_GridCells->SetHiddenInGame(false);
		MC_FloorNodes->SetHiddenInGame(false);
		MC_BottomCrossbar->SetHiddenInGame(false);

		break;

	case (int)ROOM_H2:
		GridColor = FVector(0.9f, 0.3f, 0.9f);
		FloorColor = FVector(0.2f, 0.f, 0.3f);
		GridTransparency = 0.5;
		FloorTransparency = 0.5;
		GridGlow = 10.f;
		FloorGlow = 10.f;

		GridCells_Perimeter = 30;
		GridCells_OuterR = 0.9;
		GridCells_InnerR = 0.1;
		GridCells_Screw = 0.1;

		FloorNodes_CellSize = 30.f;
		FloorNodes_Radius = 1.0f;
		FloorNodes_OuterR = 0.1;
		FloorNodes_InnerR = 0.7;

		BuildFloorNodes();
		BuildGridNodes();

		UpdateGridCells(center, radius);
		UpdateFloorNodes(center, radius);
		UpdateCrossHBars(center, radius);

		MC_GridCells->SetHiddenInGame(false);
		MC_FloorNodes->SetHiddenInGame(false);
		MC_TopCrossbar->SetHiddenInGame(false);

		break;

	case (int)ROOM_MM:
		// UpdateBoundaryRing(center, radius);
		// MC_BoundaryRing->SetHiddenInGame(false);
		// if (MarioModeRoomConfig)
		// 	MarioModeRoomConfig->RestartRoomSpawners();

		GridColor = FVector(0.8f, 0.8f, 0.f);
		FloorColor = FVector(0.8f, 8.f, 0.3f);
		GridTransparency = 0.2;
		FloorTransparency = 0.2;
		GridGlow = 10.f;
		FloorGlow = 10.f;

		GridCells_Perimeter = 40;
		GridCells_OuterR = 0.5;
		GridCells_InnerR = 0.25;
		GridCells_Screw = 0.1;

		FloorNodes_CellSize = 40.f;
		FloorNodes_Radius = 1.0f;
		FloorNodes_OuterR = 0.7;
		FloorNodes_InnerR = 0.07;

		BuildFloorNodes();
		BuildGridNodes();

		UpdateGridCells(center, radius);
		UpdateFloorNodes(center, radius);

		MC_GridCells->SetHiddenInGame(false);
		MC_FloorNodes->SetHiddenInGame(false);
		break;

	case (int)ROOM_FF:
		UpdateBoundaryRing(center, radius);
		MC_BoundaryRing->SetHiddenInGame(false);
		// if (FreeFallRoomConfig)
		// 	FreeFallRoomConfig->RestartRoomSpawners();

		break;	
	
	default:

		break;
	}
}

// Called when the game starts or when spawned
void ARoomSwitcher::BeginPlay()
{
	Super::BeginPlay();

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABoundaryFinder::StaticClass());

	if (nullptr == BoundaryFinder){
		BoundaryFinder = Cast<ABoundaryFinder>(FoundActor);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                            FString::Printf(TEXT(" BoundaryFinder %d, NodeSize %f"), 
							BoundaryFinder,BoundaryFinder->NodeSize));	
	}
	
	BuildGridNodes();
	BuildFloorNodes();
	// ChangeEnvironment();
	// SwitchRoom();

	float ShowStartRoomTime = 0.5f;
	FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, FName("SwitchRoom"));
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, ShowStartRoomTime, false);
}

int ARoomSwitcher::ChangeState()
{
	if (SST_BOUNDARY == CurrentState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, 
                TEXT("		change state: SST_BOUNDARY --> SST_ROOM_CHOOSE"));

		CurrentState = SST_ROOM_CHOOSE;
		ChangeEnvironment();
		BoundaryFinder->Stop();
		return CurrentState;
	} 

	if (SST_ROOM_CHOOSE == CurrentState ){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, 
                TEXT("		change state: SST_ROOM_CHOOSE --> SST_FLIGHT"));
		CurrentState = SST_FLIGHT;

		if (ROOM_MM == CurrentRoom && MarioModeRoomConfig)
			MarioModeRoomConfig->RestartRoomSpawners();

		if (ROOM_FF == CurrentRoom && FreeFallRoomConfig)
			FreeFallRoomConfig->RestartRoomSpawners();
		return CurrentState;
	}

	if (SST_FLIGHT == CurrentState )
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, 
                TEXT("		change state: SST_FLIGHT --> SST_ROOM_CHOOSE"));

		CurrentRoom = (int)ROOM_NUM - 1;
		SwitchRoom();
		CurrentState = SST_ROOM_CHOOSE;
		// BoundaryFinder->Reset();
		return CurrentState;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
                TEXT("		WRONG STATE in changeState"));

	return -1;
}

void ARoomSwitcher::ChangeEnvironment()
{
	switch (CurrentState)
	{
	case SST_BOUNDARY:
		if (BoundaryFinder){
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
                TEXT("		Reset boundary finder"));
			BoundaryFinder->Reset();
		}
		break;
	case SST_ROOM_CHOOSE:
		
		SwitchRoom();
		break;
	default:
		break;
	}
}



// Called every frame
void ARoomSwitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static long int delay = 0;
	delay++;
	if (delay < 60) return;
	if (ROOM_H2 != CurrentRoom) return;
	// if (SST_FLIGHT != CurrentState) return;
	if (!BoundaryFinder) return;
	GridCells_Screw += 0.01;

	// // FloorNodes_CellSize = 100.f;
	// // FloorNodes_Radius = 1.0f;
	// // FloorNodes_OuterR = 0.1;
	// // FloorNodes_InnerR = 0.02;

	float radius = DefaultRadius;
	FVector center = DefaultCenter;
	BoundaryFinder->GetCircle(center, radius);
	UpdateGridCells(center, radius);

	// // UpdateFloorNodes(center, radius);
	// // MC_GridCells->SetHiddenInGame(false);
	// // MC_FloorNodes->SetHiddenInGame(false);
}

