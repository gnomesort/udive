// Fill out your copyright notice in the Description page of Project Settings.


#include "BoundaryFinder.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoundaryFinder::ABoundaryFinder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PTrajectoryMesh = CreateDefaultSubobject<UProceduralMeshComponent>("TrajectoryMesh");
	PTrajectoryMesh->SetupAttachment(GetRootComponent());
	PTrajectoryMesh->SetWorldLocation(FVector(0));

	/* create trajectory node mesh */
	float r = NodeSize;
	float sq2 = UKismetMathLibrary::Sqrt(2.f);
	float sq3 = UKismetMathLibrary::Sqrt(3.f);

	/* tetrahedron */
	TNodeMeshVertices.Add(FVector(r * 2.f * sq2/3.f, r *         0, r *  -1.f/3.f));
	TNodeMeshVertices.Add(FVector(r *      -sq2/3.f, r *   sq2/sq3, r *  -1.f/3.f));
	TNodeMeshVertices.Add(FVector(r *      -sq2/3.f, r *  -sq2/sq3, r *  -1.f/3.f));
	TNodeMeshVertices.Add(FVector(r *           0.f, r *       0.f, r *   1.f    ));

	TNodeMeshTriangles.Add(0); TNodeMeshTriangles.Add(1); TNodeMeshTriangles.Add(2);
	TNodeMeshTriangles.Add(0); TNodeMeshTriangles.Add(3); TNodeMeshTriangles.Add(1);
	TNodeMeshTriangles.Add(1); TNodeMeshTriangles.Add(3); TNodeMeshTriangles.Add(2);
	TNodeMeshTriangles.Add(2); TNodeMeshTriangles.Add(3); TNodeMeshTriangles.Add(0);

	PCircleMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CircleMesh");
	PCircleMesh->SetupAttachment(GetRootComponent());
	PCircleMesh->SetWorldLocation(FVector(0));

	this->SetActorLocation(FVector(0));
}


// Called when the game starts or when spawned
void ABoundaryFinder::BeginPlay()
{
	Super::BeginPlay();
	this->SetActorLocation(FVector(0));
	PTrajectoryMesh->SetWorldLocation(FVector(0));

	// this->SetActorLocation(FVector(0));
}

void ABoundaryFinder::UpdateCircleMesh(FVector Center, float Radius)
{
	PCircleMesh->ClearAllMeshSections();
	CircleMeshVertices.Empty();
	CircleMeshTriangles.Empty();

	CircleMeshVertices.Add(Center);

	for (int i = 0; i < Perimeter; i++){
		float ang = 2 * PI * (float)i/(float)Perimeter;
		CircleMeshVertices.Add(FVector(Center.X + Radius * UKismetMathLibrary::Cos(ang),
									   Center.Y + Radius * UKismetMathLibrary::Sin(ang),
									   Center.Z ));

		CircleMeshTriangles.Add(i + 1); 
		CircleMeshTriangles.Add(0); 
		CircleMeshTriangles.Add((i + 1) % Perimeter + 1);
	}
		
	PCircleMesh->CreateMeshSection(0, CircleMeshVertices, 
									  CircleMeshTriangles, 
									  TArray<FVector>(), 
									  TArray<FVector2D>(),
									  TArray<FColor>(), 
									  TArray<FProcMeshTangent>(), false);	
}

bool ABoundaryFinder::GetCircle3P(const FVector2D& A, const FVector2D& B, const FVector2D& C, 
									FVector2D& Center, float& Radius)
{
    float D = 2.0f * ((A.X * (B.Y - C.Y)) + (B.X * (C.Y - A.Y)) + (C.X * (A.Y - B.Y)));
    if (D == 0.0f)
    {
        return false; // Points are collinear, circle does not exist
    }

    float CenterX = ((A.SizeSquared() * (B.Y - C.Y)) + (B.SizeSquared() * (C.Y - A.Y)) + (C.SizeSquared() * (A.Y - B.Y))) / D;
    float CenterY = ((A.SizeSquared() * (C.X - B.X)) + (B.SizeSquared() * (A.X - C.X)) + (C.SizeSquared() * (B.X - A.X))) / D;
    Center = FVector2D(CenterX, CenterY);

    Radius = (Center - A).Size();

    return true;
}

void ABoundaryFinder::Reset()
{
	PCircleMesh->ClearAllMeshSections();
	CircleMeshVertices.Empty();
	CircleMeshTriangles.Empty();

	PTrajectoryMesh->ClearAllMeshSections();
	TrajectoryMeshVertices.Empty();
	TrajectoryMeshTriangles.Empty();

	TrajectoryNodes.Empty();
	UpdateCenter = true;
	Active = true;
}

void ABoundaryFinder::Stop()
{
	PCircleMesh->ClearAllMeshSections();
	CircleMeshVertices.Empty();
	CircleMeshTriangles.Empty();

	PTrajectoryMesh->ClearAllMeshSections();
	TrajectoryMeshVertices.Empty();
	TrajectoryMeshTriangles.Empty();

	TrajectoryNodes.Empty();
	Active = false;
}

void ABoundaryFinder::GetCircle(FVector& Center, float& Radius)
{
	Center = CurrentCenter;
	Radius = CurrentRadius;
}

void ABoundaryFinder::EsimateCircle(FVector NewNode, FVector& Center, float& Radius)
{
	float minRadius = 1e6;
	
	FVector targetCenter(0);
	if (TrajectoryNodes.Num() < 3)
		return;
	if (TrajectoryNodes.Num() == 3){
		FVector2D C;
		GetCircle3P(FVector2D(TrajectoryNodes[0].X, TrajectoryNodes[0].Y), 
					FVector2D(TrajectoryNodes[1].X, TrajectoryNodes[1].Y),
					FVector2D(TrajectoryNodes[2].X, TrajectoryNodes[2].Y),
					C,
					Radius);
		Center = FVector(C.X, C.Y, 0.f);
		
	} else {
		int N = TrajectoryNodes.Num();
		
		for (int i = 0; i < N - 3; i++)
			for (int j = i + 1; j < N - 2; j++)
				for (int l = j + 1; l < N - 1; l++)
			{
				float R = 0;
				
				FVector2D C2D(0);

				GetCircle3P(FVector2D(TrajectoryNodes[i].X, TrajectoryNodes[i].Y), 
							FVector2D(TrajectoryNodes[j].X, TrajectoryNodes[j].Y),
							// FVector2D(TrajectoryNodes[N-1].X, TrajectoryNodes[N-1].Y),
							FVector2D(TrajectoryNodes[l].X, TrajectoryNodes[l].Y),
							C2D, R);
				FVector C(C2D.X, C2D.Y, 0.f);

				bool allInside = true;
				/* check if we have outsiders */
				for (int k = 0; k < N; k++)
				{
					// if (k == i || k == j || k == N-1) continue;
					if (k == i || k == j || k == l) continue;
					if ((TrajectoryNodes[k].X - C.X) * (TrajectoryNodes[k].X - C.X) +
						(TrajectoryNodes[k].Y - C.Y) * (TrajectoryNodes[k].Y - C.Y) > R * R){
						allInside = false;
						break;
					}
				}
				if (allInside != true)
					continue;
				else 
				{
					if (minRadius > R){
						targetCenter = C;
						minRadius = R;
					}
				}
			}
	}

	if (minRadius < MaxRadius){
		CurrentRadius = minRadius;
		CurrentCenter = targetCenter;
		Radius = minRadius;
		Center = targetCenter;
	}
}

void ABoundaryFinder::AddTrajectoryNode(FVector node)
{
	TrajectoryNodes.Add(node);	
	if (TrajectoryNodes.Num() > TrajectoryNodsLimit){
		TrajectoryNodes.RemoveAt(0);

		for (int i = 0; i < TNodeMeshVertices.Num(); i++)
			TrajectoryMeshVertices.RemoveAt(0);	
	}
	
	FVector nodeZ0(node.X, node.Y, 10.f);
	int n = TNodeMeshVertices.Num();
	for (int i = 0; i < TNodeMeshVertices.Num(); i++)
		TrajectoryMeshVertices.Add(nodeZ0 + TNodeMeshVertices[i]);

	if (TrajectoryNodes.Num() <= TrajectoryNodsLimit)
	{
		for (int i = 0; i < TNodeMeshTriangles.Num(); i++)
		{
			TrajectoryMeshTriangles.Add(TNodeMeshTriangles[i] + n * (TrajectoryNodes.Num()-1));

			// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, 
			// 			FString::Printf(TEXT("idx[%d]  %d"), i, TrajectoryMeshTriangles[i]));	

		}
	}
	
	if (TrajectoryNodes.Num() > 1){
		PTrajectoryMesh->ClearAllMeshSections();
	}

	PTrajectoryMesh->CreateMeshSection(0, TrajectoryMeshVertices, 
									      TrajectoryMeshTriangles, 
									      TArray<FVector>(), 
									      TArray<FVector2D>(), //UV0
									      TArray<FColor>(), 
									      TArray<FProcMeshTangent>(), false);	
}


void ABoundaryFinder::SetDefaultCenter(FVector Center){
	CurrentCenter = Center;
	DefaultCenter = Center;
}

void ABoundaryFinder::Update(FVector head)
{
	if (!Active) return;

	if (UpdateCenter){
		CurrentRadius = DefaultRadius;
		CurrentCenter = head;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, 
						FString::Printf(TEXT("Update Center , Radius %f"), CurrentRadius));	

		UpdateCircleMesh(FVector(CurrentCenter.X, CurrentCenter.Y, 10.f), CurrentRadius);
		UpdateCenter = false;
	} else if ((PreviousNodePosition - head).Length() > Density)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, 
		// 				FString::Printf(TEXT("PreviousNodePosition - head).Length() > Density")));	

		AddTrajectoryNode(head);
		PreviousNodePosition = head;

		float R = 0.f;
		FVector C(0.f);
		EsimateCircle(head, C, R);
		UpdateCircleMesh(FVector(C.X, C.Y, 10.f), R);
	}
}



// Called every frame
void ABoundaryFinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

