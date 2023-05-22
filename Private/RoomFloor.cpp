#include "RoomSwitcher.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ARoomSwitcher::BuildFloorNodes()
{
	/* star for floor cell  (in XoY plane) */
	FloorNodes_NodeVertices.Empty();
	FloorNodes_NodeTriangles.Empty();

	float sR = FloorNodes_OuterR * FloorNodes_CellSize;
	float sr = FloorNodes_InnerR * FloorNodes_CellSize;

	FloorNodes_NodeVertices.Add(FVector(0.f, 0.f, 0.f));
	FloorNodes_NodeVertices.Add(FVector(-sr, -sr, 0.f));
	FloorNodes_NodeVertices.Add(FVector(-sr,  sr, 0.f));
	FloorNodes_NodeVertices.Add(FVector( sr,  sr, 0.f));
	FloorNodes_NodeVertices.Add(FVector( sr, -sr, 0.f));
	FloorNodes_NodeVertices.Add(FVector(-sR, 0.f, 0.f));
	FloorNodes_NodeVertices.Add(FVector(0.f,  sR, 0.f));
	FloorNodes_NodeVertices.Add(FVector( sR,  0., 0.f));
	FloorNodes_NodeVertices.Add(FVector(0.f, -sR, 0.f));
	
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(8); FloorNodes_NodeTriangles.Add(1);
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(1); FloorNodes_NodeTriangles.Add(5);
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(5); FloorNodes_NodeTriangles.Add(2);
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(2); FloorNodes_NodeTriangles.Add(6);
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(6); FloorNodes_NodeTriangles.Add(3);
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(3); FloorNodes_NodeTriangles.Add(7);
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(7); FloorNodes_NodeTriangles.Add(4);
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(4); FloorNodes_NodeTriangles.Add(8);

	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(1); FloorNodes_NodeTriangles.Add(8); 
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(5); FloorNodes_NodeTriangles.Add(1); 
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(2); FloorNodes_NodeTriangles.Add(5); 
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(6); FloorNodes_NodeTriangles.Add(2); 
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(3); FloorNodes_NodeTriangles.Add(6); 
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(7); FloorNodes_NodeTriangles.Add(3); 
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(4); FloorNodes_NodeTriangles.Add(7); 
	FloorNodes_NodeTriangles.Add(0); FloorNodes_NodeTriangles.Add(8); FloorNodes_NodeTriangles.Add(4); 
}

void ARoomSwitcher::UpdateFloorNodes(FVector Center, float Radius)
{
	TArray<FVector> FloorVertices; 
	TArray<int> FloorTriangles;

	float r = FloorNodes_Radius * Radius;
	int nodeIdx = 0;
	for (float x = -r; x < r; x += FloorNodes_CellSize)
		for (float y = -r; y < r; y += FloorNodes_CellSize)
		{
			if (x * x + y * y > r * r) continue;
			FVector node( x, y, hOffset );
			
			for (int i = 0; i < FloorNodes_NodeVertices.Num(); i++){
				FloorVertices.Add(node + FloorNodes_NodeVertices[i]);
			}

			for (int i = 0; i < FloorNodes_NodeTriangles.Num(); i++){
				FloorTriangles.Add(FloorNodes_NodeTriangles[i] + nodeIdx * FloorNodes_NodeVertices.Num());
			}
			nodeIdx++;				
		}

	MC_FloorNodes->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_FloorNodes->ClearAllMeshSections();
	MC_FloorNodes->CreateMeshSection(0, FloorVertices, 
										FloorTriangles, 
										TArray<FVector>(), 
										TArray<FVector2D>(),
										TArray<FColor>(), 
										TArray<FProcMeshTangent>(), false);	
}

void ARoomSwitcher::UpdateFloorHex(FVector Center, float Radius)
{
    TArray<FVector> FloorVertices; 
	TArray<int> FloorTriangles;
    
    float r = HexTileSize + HexTileGap;
	float hx = 3.f * r;
	float hy = UKismetMathLibrary::Sqrt(3.f) * r;
	int n = 2 * (int)((HexFloorRadius * Radius + 1e-5) / hx);
	int m = 2 * (int)((HexFloorRadius * Radius + 1e-5) / hy);
	int trianIdx = 0;
	int tileIdx = 0;
	for (int i = -n + 1; i < n; i++)
	{
		for (int j = -m + 1; j < m; j++)
		{
			for (int odd = 0; odd < 2; odd++)
			{
				FVector p = odd == 0 ? FVector(i * hx, j * hy, hOffset) : FVector(i * hx + hx/2.f, j * hy + hy/2.f, hOffset);
				// if(GEngine)
				// 	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, 
				// 		FString::Printf(TEXT("p %f %f 0.f  Length %f"), p.X, p.Y, p.Length()));

				if (p.Length() < HexFloorRadius * Radius)
				{
					FloorVertices.Add(FVector(p.X, p.Y, p.Z));
					for (int k = 0; k < 6; k++)
					{
						FloorVertices.Add(FVector(
							p.X + HexTileSize * UKismetMathLibrary::Cos(2 * PI * (float)k/6.f),
							p.Y + HexTileSize * UKismetMathLibrary::Sin(2 * PI * (float)k/6.f),
							hOffset));

						FloorTriangles.Add(tileIdx * 7);
						FloorTriangles.Add(tileIdx * 7 + k % 6 + 1);
						FloorTriangles.Add(tileIdx * 7 + (k + 1) % 6 + 1);

						FloorTriangles.Add(tileIdx * 7 + (k + 1) % 6 + 1);
						FloorTriangles.Add(tileIdx * 7 + k % 6 + 1);
						FloorTriangles.Add(tileIdx * 7);
					}
					tileIdx++;
				}
			}
		}
	}

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, 
			FString::Printf(TEXT("Tile Num %d n = %d m = %d"), tileIdx, n, m));

    MC_FloorHex->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_FloorHex->ClearAllMeshSections();
	MC_FloorHex->CreateMeshSection(0, FloorVertices, 
								FloorTriangles, 
								TArray<FVector>(), 
								TArray<FVector2D>(),
								TArray<FColor>(), 
								TArray<FProcMeshTangent>(), false);	
}