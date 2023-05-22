// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowObjectGG.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFlowObjectGG::AFlowObjectGG()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProMesh");
	PMesh->SetupAttachment(GetRootComponent());

	FloorMesh = CreateDefaultSubobject<UProceduralMeshComponent>("FloorMesh");
	FloorMesh->SetupAttachment(GetRootComponent());
}

void AFlowObjectGG::BuildGuardianGrid(TArray<FVector> Points)
{
	float ZzzOffset = 0.f;
	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, FString::Printf(TEXT("GGPoints num %d"),Points.Num()));	

	if (Points.Num() == 0)
	{
		float n = 200.f;
		float r = 300.f;
		for (float i = 0; i < n; i+=1.f)
		{
			Vertices.Add(FVector(
				r * UKismetMathLibrary::Cos(2 * PI * i/n),
				r * UKismetMathLibrary::Sin(2 * PI * i/n),
				HeightBottom));
			Vertices.Add(FVector(
				r * UKismetMathLibrary::Cos(2 * PI * i/n),
				r * UKismetMathLibrary::Sin(2 * PI * i/n),
				HeightTop));
		}
	} else 
	{
		for (int i = 0; i < Points.Num(); i++)
		{
			Vertices.Add(FVector(Points[i].X, Points[i].Y, HeightBottom));
			Vertices.Add(FVector(Points[i].X, Points[i].Y, HeightTop));
		}
	}
		
	for (int i = 0; i < Vertices.Num()/4; i++)
	{
		Triangles.Add(i * 4 + 2);	
		Triangles.Add(i * 4 + 1);	
		Triangles.Add(i * 4 + 0);	

		Triangles.Add(i * 4 + 1);	
		Triangles.Add(i * 4 + 2);	
		Triangles.Add(i * 4 + 3);	

		Triangles.Add(i * 4 + 0);	
		Triangles.Add(i * 4 + 1);	
		Triangles.Add(i * 4 + 2);	

		Triangles.Add(i * 4 + 3);	
		Triangles.Add(i * 4 + 2);	
		Triangles.Add(i * 4 + 1);	
	}

	// PMesh->CreateMeshSection(0, Vertices, 
	// 							Triangles, 
	// 							TArray<FVector>(), 
	// 							TArray<FVector2D>(), //UV0
	// 							TArray<FColor>(), 
	// 							TArray<FProcMeshTangent>(), false);	

    int FloorOffsetIndex = Vertices.Num() - 1;
	/*  Floor mesh */
	float r = HexTileSize + HexTileGap;
	float hx = 3.f * r;
	float hy = UKismetMathLibrary::Sqrt(3.f) * r;
	int n = 2 * (int)((FloorRadius + 1e-5) / hx);
	int m = 2 * (int)((FloorRadius + 1e-5) / hy);
	int trianIdx = 0;
	int tileIdx = 0;
	for (int i = -n + 1; i < n; i++)
	{
		for (int j = -m + 1; j < m; j++)
		{
			for (int odd = 0; odd < 2; odd++)
			{
				FVector p = odd == 0 ? FVector(i * hx, j * hy, ZzzOffset) : FVector(i * hx + hx/2.f, j * hy + hy/2.f, ZzzOffset);
				// if(GEngine)
				// 	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, 
				// 		FString::Printf(TEXT("p %f %f 0.f  Length %f"), p.X, p.Y, p.Length()));

				if (p.Length() < FloorRadius)
				{
					FloorVertices.Add(FVector(p.X, p.Y, p.Z));
                    Vertices.Add(FVector(p.X, p.Y, p.Z));

					for (int k = 0; k < 6; k++)
					{
						FloorVertices.Add(FVector(
							p.X + HexTileSize * UKismetMathLibrary::Cos(2 * PI * (float)k/6.f),
							p.Y + HexTileSize * UKismetMathLibrary::Sin(2 * PI * (float)k/6.f),
							ZzzOffset));

                        Vertices.Add(FVector(
							p.X + HexTileSize * UKismetMathLibrary::Cos(2 * PI * (float)k/6.f),
							p.Y + HexTileSize * UKismetMathLibrary::Sin(2 * PI * (float)k/6.f),
							ZzzOffset));

						// FloorTriangles.Add(tileIdx * 7);
						// FloorTriangles.Add(tileIdx * 7 + k % 6 + 1);
						// FloorTriangles.Add(tileIdx * 7 + (k + 1) % 6 + 1);

						// FloorTriangles.Add(tileIdx * 7 + (k + 1) % 6 + 1);
						// FloorTriangles.Add(tileIdx * 7 + k % 6 + 1);
						// FloorTriangles.Add(tileIdx * 7);

                        Triangles.Add(FloorOffsetIndex + tileIdx * 7);
						Triangles.Add(FloorOffsetIndex + tileIdx * 7 + k % 6 + 1);
						Triangles.Add(FloorOffsetIndex + tileIdx * 7 + (k + 1) % 6 + 1);
						Triangles.Add(FloorOffsetIndex + tileIdx * 7 + (k + 1) % 6 + 1);
						Triangles.Add(FloorOffsetIndex + tileIdx * 7 + k % 6 + 1);
						Triangles.Add(FloorOffsetIndex + tileIdx * 7);
					}
					tileIdx++;
				}
			}
		}
	}

	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, 
	// 		FString::Printf(TEXT("FLOWWWW   Tile Num %d n = %d m = %d"), tileIdx, n, m));


    PMesh->CreateMeshSection(0, Vertices, 
								Triangles, 
								TArray<FVector>(), 
								TArray<FVector2D>(), //UV0
								TArray<FColor>(), 
								TArray<FProcMeshTangent>(), false);	
                                
	// FloorMesh->CreateMeshSection(0, FloorVertices, 
	// 							FloorTriangles, 
	// 							TArray<FVector>(), 
	// 							TArray<FVector2D>(),
	// 							TArray<FColor>(), 
	// 							TArray<FProcMeshTangent>(), false);	
}

// Called when the game starts or when spawned
void AFlowObjectGG::BeginPlay()
{
	Super::BeginPlay();
}