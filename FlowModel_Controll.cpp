// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowModel_Controll.h"

AFlowModel_Controll::AFlowModel_Controll()
{

}

FTransform AFlowModel_Controll::GetIncrementTransform(float TimeFromNow, float DeltaTime, FVector P)
{
    FTransform res;
    res.SetTranslation(UpVector + RightVector + ForwardVector);
    return res;
}

void AFlowModel_Controll::SetUpVector(FVector UpVectorArg)
{
    UpVector = UpVectorArg;
}

void AFlowModel_Controll::SetRightVector(FVector RightVectorArg)
{
    RightVector = RightVectorArg;
}

void AFlowModel_Controll::SetForwardVector(FVector ForwardVectorArg)
{
    ForwardVector = ForwardVectorArg;
}

void AFlowModel_Controll::SetSpeed(float SpeedArg)
{
    Speed = SpeedArg;
}
