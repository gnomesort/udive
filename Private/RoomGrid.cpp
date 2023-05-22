#include "RoomSwitcher.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ARoomSwitcher::BuildGridNodes()
{
	/* create trajectory node mesh */
	float r = GridDots_NodeSize;
	float sq2 = UKismetMathLibrary::Sqrt(2.f);
	float sq3 = UKismetMathLibrary::Sqrt(3.f);

	/* tetrahedron */
	GridDots_NodeVertices.Empty();
	GridDots_NodeTriangles.Empty();

	GridDots_NodeVertices.Add(FVector(r * 2.f * sq2/3.f, r *         0, r *  -1.f/3.f));
	GridDots_NodeVertices.Add(FVector(r *      -sq2/3.f, r *   sq2/sq3, r *  -1.f/3.f));
	GridDots_NodeVertices.Add(FVector(r *      -sq2/3.f, r *  -sq2/sq3, r *  -1.f/3.f));
	GridDots_NodeVertices.Add(FVector(r *           0.f, r *       0.f, r *   1.f    ));

	GridDots_NodeTriangles.Add(0); GridDots_NodeTriangles.Add(1); GridDots_NodeTriangles.Add(2);
	GridDots_NodeTriangles.Add(0); GridDots_NodeTriangles.Add(3); GridDots_NodeTriangles.Add(1);
	GridDots_NodeTriangles.Add(1); GridDots_NodeTriangles.Add(3); GridDots_NodeTriangles.Add(2);
	GridDots_NodeTriangles.Add(2); GridDots_NodeTriangles.Add(3); GridDots_NodeTriangles.Add(0);

	/* star for cell grid (in XoZ plane) */
	GridCells_NodeVertices.Empty();
	GridCells_NodeTriangles.Empty();

	float sR = GridCells_OuterR * 2.f * PI / GridDots_Perimeter; // and then * Radius
	float sr = GridCells_InnerR * 2.f * PI / GridDots_Perimeter; // and then * Radius

	GridCells_NodeVertices.Add(FVector(0.f, 0.f, 0.f));
	GridCells_NodeVertices.Add(FVector(-sr, 0.f, -sr));
	GridCells_NodeVertices.Add(FVector(-sr, 0.f,  sr));
	GridCells_NodeVertices.Add(FVector( sr, 0.f,  sr));
	GridCells_NodeVertices.Add(FVector( sr, 0.f, -sr));
	GridCells_NodeVertices.Add(FVector(-sR, 0.f, 0.f));
	GridCells_NodeVertices.Add(FVector(0.f, 0.f,  sR));
	GridCells_NodeVertices.Add(FVector( sR, 0.f,  0.f));
	GridCells_NodeVertices.Add(FVector(0.f, 0.f, -sR));
	
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(8); GridCells_NodeTriangles.Add(1);
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(1); GridCells_NodeTriangles.Add(5);
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(5); GridCells_NodeTriangles.Add(2);
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(2); GridCells_NodeTriangles.Add(6);
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(6); GridCells_NodeTriangles.Add(3);
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(3); GridCells_NodeTriangles.Add(7);
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(7); GridCells_NodeTriangles.Add(4);
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(4); GridCells_NodeTriangles.Add(8);

	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(1); GridCells_NodeTriangles.Add(8); 
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(5); GridCells_NodeTriangles.Add(1); 
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(2); GridCells_NodeTriangles.Add(5); 
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(6); GridCells_NodeTriangles.Add(2); 
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(3); GridCells_NodeTriangles.Add(6); 
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(7); GridCells_NodeTriangles.Add(3); 
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(4); GridCells_NodeTriangles.Add(7); 
	GridCells_NodeTriangles.Add(0); GridCells_NodeTriangles.Add(8); GridCells_NodeTriangles.Add(4); 
}


void ARoomSwitcher::UpdateGrid(int GridId, FVector Center, float Radius)
{
}

void ARoomSwitcher::UpdateGridDots(FVector Center, float Radius)
{
    TArray<FVector> GridVertices; 
	TArray<int> GridTriangles;

	GridVertices.Empty();
	GridTriangles.Empty();

	float dAng = 2.f * PI / GridDots_Perimeter;
	float dH = dAng * Radius;

	int nodeIdx = 0;
	float screw = 0.f;
	for (float z = LowerBoundary; z < UpperBoundary; z += dH)
	{
		screw += GridDots_Screw * dAng;
		for (float a = 0; a < 2.f * PI - 1e-5; a += dAng)
		{
			FVector node( Radius * UKismetMathLibrary::Cos(a + screw),
						  Radius * UKismetMathLibrary::Sin(a + screw),
						  z );
			
			for (int i = 0; i < GridDots_NodeVertices.Num(); i++){
				GridVertices.Add(node + GridDots_NodeVertices[i]);
			}

			for (int i = 0; i < GridDots_NodeTriangles.Num(); i++){
				GridTriangles.Add(GridDots_NodeTriangles[i] + nodeIdx * GridDots_NodeVertices.Num());
			}
			nodeIdx++;
		}
	}

	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 10.5, FColor::Blue, 
	// 				FString::Printf(TEXT("GridVertices %d,  GridTriangles  %d"), 
	// 						GridVertices.Num(), GridTriangles.Num()));	
	
	MC_GridDots->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_GridDots->ClearAllMeshSections();
	MC_GridDots->CreateMeshSection(0, GridVertices, 
									GridTriangles, 
									TArray<FVector>(), 
									TArray<FVector2D>(),
									TArray<FColor>(), 
									TArray<FProcMeshTangent>(), false);	
}

void ARoomSwitcher::UpdateGridCells(FVector Center, float Radius)
{
	TArray<FVector> GridVertices; 
	TArray<int> GridTriangles;

	GridVertices.Empty();
	GridTriangles.Empty();

	float dAng = 2.f * PI / GridCells_Perimeter;
	float dH = dAng * Radius;

	int nodeIdx = 0;
	float screw = 0.f;
	for (float z = LowerBoundary; z < UpperBoundary; z += dH)
	{
		// screw += GridCells_Screw * dAng;
		screw += UKismetMathLibrary::Sin(z*0.023) * GridCells_Screw * dAng;

		for (float a = 0; a < 2.f * PI - 1e-5; a += dAng)
		{
			float cs = Radius * UKismetMathLibrary::Cos(a + screw);
			float sn = Radius * UKismetMathLibrary::Sin(a + screw);
			FVector node( cs, sn, z );
			
			for (int i = 0; i < GridCells_NodeVertices.Num(); i++){

				GridVertices.Add(FVector(
					 node.X + (GridCells_NodeVertices[i].Y) * cs - (GridCells_NodeVertices[i].X) * sn,
					 node.Y + (GridCells_NodeVertices[i].Y) * sn + (GridCells_NodeVertices[i].X) * cs,
					 node.Z + (Radius * GridCells_NodeVertices[i].Z)
				));
			}

			for (int i = 0; i < GridCells_NodeTriangles.Num(); i++){
				GridTriangles.Add(GridCells_NodeTriangles[i] + nodeIdx * GridCells_NodeVertices.Num());
			}
			nodeIdx++;
		}
	}

	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 10.5, FColor::Blue, 
	// 				FString::Printf(TEXT("GridVertices %d,  GridTriangles  %d"), 
	// 						GridVertices.Num(), GridTriangles.Num()));	
	
	MC_GridCells->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_GridCells->ClearAllMeshSections();
	MC_GridCells->CreateMeshSection(0, GridVertices, 
									GridTriangles, 
									TArray<FVector>(), 
									TArray<FVector2D>(),
									TArray<FColor>(), 
									TArray<FProcMeshTangent>(), false);	
}



void ARoomSwitcher::UpdateGridRods(FVector Center, float Radius)
{
    TArray<FVector> GridVertices; 
	TArray<int> GridTriangles;

	GridVertices.Empty();
	GridTriangles.Empty();

	float dAng = 2.f * PI / GridRods_Perimeter;

	for (float i = 0; i < GridRods_Perimeter; i+=1.f)
	{
		GridVertices.Add(FVector(
			Radius * UKismetMathLibrary::Cos(i * dAng),
			Radius * UKismetMathLibrary::Sin(i * dAng),
			LowerBoundary));
		GridVertices.Add(FVector(
			Radius * UKismetMathLibrary::Cos(i * dAng),
			Radius * UKismetMathLibrary::Sin(i * dAng),
			UpperBoundary));
	}
	for (int i = 0; i < GridVertices.Num()/4; i++)
	{
		GridTriangles.Add(i * 4 + 2);	
		GridTriangles.Add(i * 4 + 1);	
		GridTriangles.Add(i * 4 + 0);	

		GridTriangles.Add(i * 4 + 1);	
		GridTriangles.Add(i * 4 + 2);	
		GridTriangles.Add(i * 4 + 3);	

		GridTriangles.Add(i * 4 + 0);	
		GridTriangles.Add(i * 4 + 1);	
		GridTriangles.Add(i * 4 + 2);	

		GridTriangles.Add(i * 4 + 3);	
		GridTriangles.Add(i * 4 + 2);	
		GridTriangles.Add(i * 4 + 1);	
	}
		
	MC_GridRods->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_GridRods->ClearAllMeshSections();
	MC_GridRods->CreateMeshSection(0, GridVertices, 
									GridTriangles, 
									TArray<FVector>(), 
									TArray<FVector2D>(),
									TArray<FColor>(), 
									TArray<FProcMeshTangent>(), false);	
}