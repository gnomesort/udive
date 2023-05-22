#include "RoomSwitcher.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ARoomSwitcher::UpdateElements(FVector Center, float Radius)
{
	// UpdateGridRods(Center, Radius);
	// UpdateGridDots(Center, Radius);
	// UpdateGridCells(Center, Radius);
	// UpdateFloorHex(Center, Radius);
	// UpdateFloorNodes(Center, Radius);
}

void ARoomSwitcher::UpdateCrossVBars(FVector Center, float Radius)
{
    TArray<FVector> CBarVertices; 
    TArray<FVector> LBarVertices; 
    TArray<FVector> RBarVertices; 
	TArray<int> BarTriangles;
    TArray<FVector2D> UVs;

	CBarVertices.Empty();
    LBarVertices.Empty();
    RBarVertices.Empty();
	BarTriangles.Empty();
    UVs.Empty();

    float barPerimeter = 24.f;
	float dAng = 2.f * PI / barPerimeter;

    FVector endBL = FVector(0.f - Radius * SideCrossbarShift, 0.f, 0.f);
    FVector endBR = FVector(0.f + Radius * SideCrossbarShift, 0.f, 0.f);
    FVector endBC = FVector(0.f, 0.f, 0.f);

    FVector endTL = FVector(0.f - Radius * SideCrossbarShift, 0.f, SideCrossbarHeight);
    FVector endTR = FVector(0.f + Radius * SideCrossbarShift, 0.f, SideCrossbarHeight);
    FVector endTC = FVector(0.f, 0.f, SideCrossbarHeight);

    int pNum = 0;
    for (float a = 0; a < 2.f * PI + 1e-5; a += dAng)
    {
        pNum++;

        // UVs.Add(FVector2D(UKismetMathLibrary::RandomFloatInRange(0.f, 100.f), 
        //                   UKismetMathLibrary::RandomFloatInRange(0.f, 100.f)));

        // UVs.Add(FVector2D(UKismetMathLibrary::RandomFloatInRange(0.f, 100.f), 
        //                   UKismetMathLibrary::RandomFloatInRange(0.f, 100.f)));

        UVs.Add(FVector2D(a, 0.f));
        UVs.Add(FVector2D(a, 75.f));


        CBarVertices.Add(FVector(endBC.X + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endBC.Y + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f, endBC.Z));
        CBarVertices.Add(FVector(endTC.X + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endTC.Y + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f, endTC.Z));

        LBarVertices.Add(FVector(endBL.X + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endBL.Y + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f, endBL.Z));
        LBarVertices.Add(FVector(endTL.X + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endTL.Y + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f, endTL.Z));

        RBarVertices.Add(FVector(endBR.X + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endBR.Y + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f, endBR.Z));
        RBarVertices.Add(FVector(endTR.X + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endTR.Y + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f, endTR.Z));
    }

    for (int i = 0; i < pNum; i++){
        BarTriangles.Add((i + 0) * 2 + 0);
        BarTriangles.Add((i + 0) * 2 + 1);
        BarTriangles.Add((i + 1) * 2 + 0);
        
        BarTriangles.Add((i + 1) * 2 + 0);
        BarTriangles.Add((i + 0) * 2 + 1);
        BarTriangles.Add((i + 1) * 2 + 1);
    }

	GEngine->AddOnScreenDebugMessage(-1, 20.5, FColor::Orange, 
					FString::Printf(TEXT("Center %f  %f :  "), 
							Center.X, Center.Y));	
	
	MC_CenterCrossbar->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_CenterCrossbar->ClearAllMeshSections();
	MC_CenterCrossbar->CreateMeshSection(0, CBarVertices, BarTriangles, TArray<FVector>(), 
                                            // TArray<FVector2D>(),
                                            UVs,
									        TArray<FColor>(), TArray<FProcMeshTangent>(), false);	

    MC_LeftCrossbar->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_LeftCrossbar->ClearAllMeshSections();
	MC_LeftCrossbar->CreateMeshSection(0, LBarVertices, BarTriangles, TArray<FVector>(), 
                                            //TArray<FVector2D>(),
                                            UVs,
									        TArray<FColor>(), TArray<FProcMeshTangent>(), false);
                                        
    MC_RightCrossbar->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_RightCrossbar->ClearAllMeshSections();
	MC_RightCrossbar->CreateMeshSection(0, RBarVertices, BarTriangles, TArray<FVector>(), 
                                            // TArray<FVector2D>(),
                                            UVs,
									        TArray<FColor>(), TArray<FProcMeshTangent>(), false);	
}

void ARoomSwitcher::UpdateBoundaryRing(FVector Center, float Radius)
{
    TArray<FVector> RingVertices; 
	TArray<int> RingTriangles;
    TArray<FVector2D> UVs;

    RingVertices.Empty();
    RingTriangles.Empty();
    UVs.Empty();

    float ringPerimeter = 64.f;
	float dAng = 2.f * PI / ringPerimeter;
    

    int pNum = 0;
    for (float a = 0; a < 2.f * PI - 1e-5; a += dAng)
    {
        pNum++;

        // UVs.Add(FVector2D(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 
        //                   UKismetMathLibrary::RandomFloatInRange(0.f, 1.f)));

        // UVs.Add(FVector2D(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 
        //                   UKismetMathLibrary::RandomFloatInRange(0.f, 1.f)));

        UVs.Add(FVector2D(a * 10.f, 0.f));
        UVs.Add(FVector2D(a * 10.f, 1.f));

        RingVertices.Add(FVector(UKismetMathLibrary::Cos(a) * Radius,
                                 UKismetMathLibrary::Sin(a) * Radius, 
                                 RingH - RingW/2.f));

        RingVertices.Add(FVector(UKismetMathLibrary::Cos(a) * Radius,
                                 UKismetMathLibrary::Sin(a) * Radius, 
                                 RingH + RingW/2.f));
    }

    for (int i = 0; i < pNum - 3; i++){
        
        RingTriangles.Add((i + 0) * 2 + 0);
        RingTriangles.Add((i + 0) * 2 + 1);
        RingTriangles.Add((i + 1) * 2 + 0);

        RingTriangles.Add((i + 1) * 2 + 0);
        RingTriangles.Add((i + 0) * 2 + 1);
        RingTriangles.Add((i + 1) * 2 + 1);
        
        RingTriangles.Add((i + 0) * 2 + 1);
        RingTriangles.Add((i + 0) * 2 + 0);
        RingTriangles.Add((i + 1) * 2 + 0);
        
        RingTriangles.Add((i + 0) * 2 + 1);
        RingTriangles.Add((i + 1) * 2 + 0);
        RingTriangles.Add((i + 1) * 2 + 1);
    }

	// GEngine->AddOnScreenDebugMessage(-1, 20.5, FColor::Orange, 
	// 				FString::Printf(TEXT("Center %f  %f :  "), 
	// 						Center.X, Center.Y));	
	
	MC_BoundaryRing->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_BoundaryRing->ClearAllMeshSections();
	MC_BoundaryRing->CreateMeshSection(0, RingVertices, RingTriangles, TArray<FVector>(), 
                                            // TArray<FVector2D>(),
                                            UVs,
									        TArray<FColor>(), TArray<FProcMeshTangent>(), false);	
}


void ARoomSwitcher::UpdateCrossHBars(FVector Center, float Radius)
{
    TArray<FVector> BBarVertices; 
    TArray<FVector> TBarVertices; 
	TArray<int> BarTriangles;
    TArray<FVector2D> UVs;

	BBarVertices.Empty();
    TBarVertices.Empty();
    BarTriangles.Empty();
    UVs.Empty();

    float barPerimeter = 24.f;
	float dAng = 2.f * PI / barPerimeter;

    FVector endBL = FVector(0.f - Radius, 0.f, BottomCrossbarHeight);
    FVector endBR = FVector(0.f + Radius, 0.f, BottomCrossbarHeight);

    FVector endTL = FVector(0.f - Radius, 0.f, TopCrossbarHeight);
    FVector endTR = FVector(0.f + Radius, 0.f, TopCrossbarHeight);

    int pNum = 0;
    for (float a = 0; a < 2.f * PI + 1e-5; a += dAng)
    {
        pNum++;

        // UVs.Add(FVector2D(UKismetMathLibrary::RandomFloatInRange(0.f, 100.f), 
        //                   UKismetMathLibrary::RandomFloatInRange(0.f, 100.f)));

        // UVs.Add(FVector2D(UKismetMathLibrary::RandomFloatInRange(0.f, 100.f), 
        //                   UKismetMathLibrary::RandomFloatInRange(0.f, 100.f)));

        UVs.Add(FVector2D(a * 0.3f, 0.f));
        UVs.Add(FVector2D(a * 0.3f, 15.f));

        BBarVertices.Add(FVector(endBL.X,
                                 endBL.Y + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endBL.Z + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f));

        BBarVertices.Add(FVector(endBR.X,
                                 endBR.Y + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endBR.Z + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f));

        TBarVertices.Add(FVector(endTL.X,
                                 endTL.Y + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endTL.Z + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f));

        TBarVertices.Add(FVector(endTR.X,
                                 endTR.Y + UKismetMathLibrary::Cos(a) * CrossbarDiam/2.f,
                                 endTR.Z + UKismetMathLibrary::Sin(a) * CrossbarDiam/2.f));

    }

    for (int i = 0; i < pNum; i++){
        BarTriangles.Add((i + 0) * 2 + 0);
        BarTriangles.Add((i + 0) * 2 + 1);
        BarTriangles.Add((i + 1) * 2 + 0);
        
        BarTriangles.Add((i + 1) * 2 + 0);
        BarTriangles.Add((i + 0) * 2 + 1);
        BarTriangles.Add((i + 1) * 2 + 1);
    }

	MC_BottomCrossbar->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_BottomCrossbar->ClearAllMeshSections();
	MC_BottomCrossbar->CreateMeshSection(0, BBarVertices, BarTriangles, TArray<FVector>(), 
                                            // TArray<FVector2D>(),
                                            UVs,
									        TArray<FColor>(), TArray<FProcMeshTangent>(), false);	

    MC_TopCrossbar->SetWorldLocation(FVector(Center.X, Center.Y, 0.f));
	MC_TopCrossbar->ClearAllMeshSections();
	MC_TopCrossbar->CreateMeshSection(0, TBarVertices, BarTriangles, TArray<FVector>(), 
                                            // TArray<FVector2D>(),
                                            UVs,
									        TArray<FColor>(), TArray<FProcMeshTangent>(), false);	
}