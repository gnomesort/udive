// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spider.generated.h"

UCLASS()
class UDIVE_API ASpider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpider();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* MeshComponent;

	/*UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* MeshComponentStatic;*/
};
