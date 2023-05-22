// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowModel_Force.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/TransformNonVectorized.h"
#include "Math/Vector.h"
#include "FlowObject.h"
#include "EngineGlobals.h"
#include "Math/UnrealMathUtility.h"

AFlowModel_Force::AFlowModel_Force()
{
	
}

// FTransform AFlowModel_Force::GetGlobalTransform(float Time, FVector P)
// {
// 	//Super::GetGlobalTransform(Time, P);
// 	FTransform res;
// 	float scale = UKismetMathLibrary::Exp(Time * UKismetMathLibrary::Loge(ScaleTimeRatio));
// 	//float scale = 2.f + UKismetMathLibrary::Sin(Time);
// 	float roll = UKismetMathLibrary::RadiansToDegrees(Time) * RollTimeRatio;
// 	res.SetTranslation(StartDirection * StartVelocity * Time);
	
// 	FRotator rrot = UKismetMathLibrary::RotatorFromAxisAndAngle(StartDirection, roll);
// 	res.SetRotation(rrot.Quaternion());
// 	res.SetScale3D(FVector(scale));

// 	return res;
// }


FTransform AFlowModel_Force::GetGlobalTransform(float Time, FVector P)
{
	float t = CurrentTime + Time;
	FTransform res;
	FTransform loc_tr;
	FTransform rot_tr;
	FTransform mesh_tr;

	FVector r = FVector(VAmp * (UKismetMathLibrary::Cos(t * VFactor) - 1.0f), 
						  t * StartVelocity, 
						  HAmp * UKismetMathLibrary::Sin(t * HFactor));

	FVector dr = FVector(-VAmp * VFactor * UKismetMathLibrary::Sin(t * VFactor), 
						  StartVelocity, 
						  HAmp * HFactor * UKismetMathLibrary::Cos(t * HFactor));

	FVector ddr = FVector::CrossProduct(r, dr);
	float cos = FVector::DotProduct(r, dr)/(r.Length() * dr.Length());
	float ang = UKismetMathLibrary::Acos(cos);

	// float Factor = (UKismetMathLibrary::Sin(1.0 * t) + 1.f) * (1.f - OrtoFactor) + OrtoFactor;
	// FRotator rot = UKismetMathLibrary::RotatorFromAxisAndAngle(ddr, ang * 180.f/PI * Factor);

	FRotator rot = UKismetMathLibrary::RotatorFromAxisAndAngle(ddr, ang * 180.f/PI * OrtoFactor);

	loc_tr.SetTranslation(r);
	Helper[0]->SetActorTransform(loc_tr);
	Helper[0]->SetActorRotation(rot.Quaternion());
	Helper[0]->SetActorScale3D(FVector(1.f));

	// FString s1 = loc_tr.GetRotation().ToString();
	// FString s2 = Helper[0]->GetTransform().GetRotation().ToString();
	// UE_LOG(LogTemp, Warning, TEXT("a  %s"), *s1);
	// UE_LOG(LogTemp, Warning, TEXT("b  %s"), *s2);
	// float logdt = 0.09;
	// if(GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, logdt, FColor::Yellow, FString::Printf(TEXT("%s"),*s1));	
	// 	GEngine->AddOnScreenDebugMessage(-1, logdt, FColor::Yellow, FString::Printf(TEXT("%s"),*s2));	
	// 	GEngine->AddOnScreenDebugMessage(-1, logdt, FColor::Red, FString::Printf(TEXT("ddr %f"), ddr.Length()));	
	// 	GEngine->AddOnScreenDebugMessage(-1, logdt, FColor::Red, FString::Printf(TEXT("dr %f"), dr.Length()));	
	// 	GEngine->AddOnScreenDebugMessage(-1, logdt, FColor::Red, FString::Printf(TEXT("ang %f"), ang));	
	// }	

	return Helper[0]->GetTransform().Inverse();
}

void AFlowModel_Force::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FTransform LocalTransform = Helper[1]->GetTransform();
	// FVector P(0.f);
	// FTransform dT = GetGlobalTransform(0, P).Inverse() * GetGlobalTransform(DeltaTime, P);
	// LocalTransform = LocalTransform * dT;
	// Helper[1]->SetActorTransform(LocalTransform);
	// Helper[1]->SetActorScale3D(FVector(2.5f));

	// FString s1 = LocalTransform.ToString();
	// float logdt = 0.09;
	// if(GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, logdt, FColor::Yellow, 
	// 		FString::Printf(TEXT("%s"), *s1));	
	// }
		
}

FTransform AFlowModel_Force::GetIncrementTransform(float TimeFromNow, float DeltaTime, FVector P)
{
	FTransform dT = GetGlobalTransform(TimeFromNow, P).Inverse() * GetGlobalTransform(TimeFromNow + DeltaTime, P);
	return dT;
}