// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowModelPwLinear.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/TransformNonVectorized.h"
#include "FlowObject.h"
#include "EngineGlobals.h"
#include "Math/UnrealMathUtility.h"
#include "RoomConfig.h"
// #include "Public/LevelConfig.h"

AFlowModelPwLinear::AFlowModelPwLinear()
{
    FMType = FMT_PWLINEAR;
}

int AFlowModelPwLinear::SetConfig(const TArray<FString>& configLines){
    return 0;
}

int AFlowModelPwLinear::SetConfig(const FString& configContent)
{
    if(GEngine){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AFlowModel::SetConfig");
    }

    // crutch
    static TMap<int, FString> FMTypeMap;

    FMTypeMap.Add(FMT_NONE, "FMT_NONE");
    FMTypeMap.Add(FMT_PWLINEAR, "FMT_PWLINEAR");

	if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FMTypeMap[FMType]);

    TArray<FString> configLines;
    configContent.ParseIntoArrayLines(configLines);

    bool correctConfig = false;
 	for (int i = 0; i < configLines.Num() - 2; i++)
    {
        FString line = configLines[i];
        if (line.Contains(FMTypeMap[FMType]))
        {
            int id = -1;
            TArray<FString> tokens;
            line.ParseIntoArray(tokens, TEXT(" "), true);

            id = (int)FCString::Atof(*tokens[1]);
            if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                            FString::Printf(TEXT(" id = %d,  FlowModelID = %d "), id, FlowModelID));	

            if (id != FlowModelID) continue;
            
            FString colsLine = configLines[i + 1];
            FString rowsLine = configLines[i + 2];

            tokens.Empty();
            colsLine.ParseIntoArray(tokens, TEXT(" "), true);
            if (tokens.Num() < 2 || !tokens.Contains("COLS")){
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                            FString::Printf(TEXT(" cols.Num() = %d "), tokens.Num()));	
                continue;
            } 
            ConfigCols = (int)FCString::Atof(*tokens[1]);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                            FString::Printf(TEXT(" ConfigCols = %d "), ConfigCols));	

            if (ConfigCols < 1) continue;

            tokens.Empty();
            rowsLine.ParseIntoArray(tokens, TEXT(" "), true);

            if (tokens.Num() < 2 || !tokens.Contains("ROWS")){
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                            FString::Printf(TEXT(" raws.Num() = %d "), tokens.Num()));	
                continue;
            } 

            ConfigRows = (int)FCString::Atof(*tokens[1]);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                            FString::Printf(TEXT(" ConfigRows = %d "), ConfigRows));	
            
            if (ConfigRows < 1) continue;

            /*  one string for config header note */
            if (i > configLines.Num() - 3 - 1 - ConfigRows) continue;

            correctConfig = true;
            for (int j = i + 4; j < i + 4 + ConfigRows; j++)
            {
                tokens.Empty();
                configLines[j].ParseIntoArray(tokens, TEXT(","), true);
                if (tokens.Num() != ConfigCols){

                    correctConfig = false;
                    break;
                }

                for (int k = 0; k < tokens.Num(); k++){
                    AConfig.Add(FCString::Atof(*tokens[k]));
                }
            }
            

            Configured = correctConfig;

            if (!correctConfig){
                AConfig.Empty();

                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
                            FString::Printf(TEXT(" incorrect config ")));	

                break;
            }

            /* check */
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
                            FString::Printf(TEXT("cols = %d, rows = %d, AConfig Size = %d"), 
                                ConfigCols, ConfigRows, AConfig.Num()));	

            // for (int k = 0; k < AConfig.Num(); k++){
            //     GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, 
            //         FString::Printf(TEXT("      val[%d] = %f"), k, AConfig[k]));	
            // }

            break;
        }
    }

    if (correctConfig){
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
                            FString::Printf(TEXT("                  :) Flow Model Config is Correct")));

        SetTrajectoryNodes();
        CalcTrajectoryNodes();
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
                            FString::Printf(TEXT("                  !!! Flow Model Incorrect Config")));
    }

    return 1;
}

int AFlowModelPwLinear::SetConfig()
{
    if (!RoomConfig) 
        return - 1;
    FString ConfigContent = RoomConfig->GetConfig();
    
    return SetConfig(ConfigContent);
}

void AFlowModelPwLinear::BeginPlay()
{
    Super::BeginPlay(); 
    
    
}

void AFlowModelPwLinear::SetTrajectoryNodes()
{
    if (!Configured) return;
    int clength = sizeof(AConfig)/sizeof(float);

    // int n = clength/ConfigCols;
    int n = AConfig.Num()/ConfigCols;
    float T = 0.f;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, 
                            FString::Printf(TEXT("n %d AConfig.Num() %d  sizeof(AConfig) = %d ConfigCols = %d"), 
                                n, AConfig.Num(), sizeof(AConfig), ConfigCols));	

    for (int i = 0; i < n; i++)
    {
        STrajectoryNode Node;
        Node.T = T; 
        Node.V = FVector(AConfig[i * ConfigCols + 0], 
                         AConfig[i * ConfigCols + 1], 
                         AConfig[i * ConfigCols + 2]); 
        
        Node.R = FVector(0.f, 0.f, 0.f);
        Node.V *= VelocityScale;
        TrajectoryNodes.Add(Node);    
        T += AConfig[i * ConfigCols + 3];
    }
}
void AFlowModelPwLinear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AFlowModelPwLinear::CalcTrajectoryNodes()
{
    if (!Configured) return;
    int n = TrajectoryNodes.Num();
    for (int i = 0; i < n - 1; i++) 
    {
        TrajectoryNodes[i + 1].R = 
        0.5f * (TrajectoryNodes[i + 1].T + TrajectoryNodes[i].T) * (TrajectoryNodes[i + 1].V - TrajectoryNodes[i].V) +
        TrajectoryNodes[i + 1].T * TrajectoryNodes[i].V - TrajectoryNodes[i].T * TrajectoryNodes[i + 1].V + 
        TrajectoryNodes[i].R;
    }
}

void AFlowModelPwLinear::GetTransformAndTilt(float TimeFromNow, FTransform & Transform, FRotator & Rotator, FVector * dR)
{
    float t = CurrentTime + TimeFromNow;
    int n = TrajectoryNodes.Num();
    int nodeIndex = 0;
    FVector r, dr, ddr;
    FTransform res;
	FTransform loc_tr;
	FTransform rot_tr;
	FTransform mesh_tr;

    // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
    //             FString::Printf(TEXT("TrajectoryNodes.Num %d"), TrajectoryNodes.Num() )); 

    if (t < TrajectoryNodes[0].T)
    {
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
        //         FString::Printf(TEXT("t : %f < TrajectoryNodes[0].T %f"), 
        //         t , TrajectoryNodes[0].T));	

        nodeIndex = -1;
        r = TrajectoryNodes[0].R + TrajectoryNodes[0].V * (t - TrajectoryNodes[0].T);
        dr = TrajectoryNodes[0].V;

    } else if (t > TrajectoryNodes[n - 1].T)
    {
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, 
        //         FString::Printf(TEXT("t : %f > TrajectoryNodes[n - 1].T %f"), 
        //         t , TrajectoryNodes[0].T));	
        nodeIndex = n;
        r = TrajectoryNodes[n - 1].R + TrajectoryNodes[n - 1].V * (t - TrajectoryNodes[n - 1].T);
        dr = TrajectoryNodes[n - 1].V;
    } else 
    {
        for (int i = 0; i < TrajectoryNodes.Num(); i++)
        {
            if (t > TrajectoryNodes[i].T && t <= TrajectoryNodes[i + 1].T)
            {
                // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
                // FString::Printf(TEXT("TrajectoryNodes[%d].T %f    < t : %f    <  TrajectoryNodes[%d].T %f"), 
                // i, TrajectoryNodes[i].T, t , i + 1, TrajectoryNodes[i + 1].T));	


                nodeIndex = i;

                FVector V0 = TrajectoryNodes[i].V;
                FVector V1 = TrajectoryNodes[i + 1].V;
                FVector R0 = TrajectoryNodes[i].R;

                float T0 = TrajectoryNodes[i].T;
                float T1 = TrajectoryNodes[i + 1].T;
                float dT = T1 - T0;

                r = 0.5f * (V1 - V0) * (t * t - T0 * T0) / dT + 
                    (T1 * V0 - T0 * V1) * (t - T0) / dT + 
                    R0;
                
                dr = (T1 - t) * V0 / dT + (t - T0) * V1 / dT;
                break;
            }
        }
    }
    

    FVector la(0.f, 0.f, -1.f);
    ddr = FVector::CrossProduct(la, dr);
	float cos = FVector::DotProduct(la, dr)/(la.Length() * dr.Length());
	float ang = UKismetMathLibrary::Acos(cos);
	Rotator = UKismetMathLibrary::RotatorFromAxisAndAngle(ddr, ang * 180.f/PI);
    loc_tr.SetTranslation(r);
	// Helper[0]->SetActorTransform(loc_tr);
	// Helper[0]->SetActorScale3D(FVector(1.f));
    // Transform = Helper[0]->GetTransform().Inverse();

    Transform = loc_tr.Inverse();

    if (dR != NULL)
        *dR = dr;
}

FTransform AFlowModelPwLinear::GetGlobalTransform(float TimeFromNow, FVector P)
{
    FTransform Transform;
    FRotator Rotator;
    GetTransformAndTilt(TimeFromNow, Transform, Rotator);
    return Transform;
}
	
FTransform AFlowModelPwLinear::GetIncrementTransform(float TimeFromNow, float DeltaTime, FVector P)
{
    FTransform dT = GetGlobalTransform(TimeFromNow, P).Inverse() * GetGlobalTransform(TimeFromNow + DeltaTime, P);
	return dT;
}