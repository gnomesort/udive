// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "FlowModel.h"
#include "FlowObject.h"
#include "RoomConfig.h"
// #include "FlowObject_Obstacle.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "Containers/Map.h"

#include <vector>

// Sets default values
ASpawner::ASpawner()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    if(GEngine){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, " -------------- ASpawner::ASpawner() -----------");
    }
    
}

int ASpawner::GetID(){
    return SpawnerID;
}

ARoomConfig * ASpawner::GetRoomConfig()
{
    return RoomConfig;
}

int ASpawner::SetConfig(const FString& configContent)
{

    if(GEngine){
		if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "SetConfig ");
    }

    // crutch
    static TMap<int, FString> STypeMap;

    STypeMap.Add(ST_NONE, "ST_NONE");
    STypeMap.Add(ST_RING, "ST_RING");
    STypeMap.Add(ST_COINS, "ST_COINS");
    STypeMap.Add(ST_GATE, "ST_GATE");
    STypeMap.Add(ST_MBLOCK, "ST_MBLOCK");
    STypeMap.Add(ST_VBLOCK, "ST_VBLOCK");
    STypeMap.Add(ST_SBLOCK, "ST_SBLOCK");
    STypeMap.Add(ST_FLOW, "ST_FLOW");
    STypeMap.Add(ST_NUM, "ST_NUM");

    if(GEngine){
		if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, STypeMap[SType]);
    }

    TArray<FString> Lines;
    configContent.ParseIntoArrayLines(Lines);

    bool correctConfig = false;
 	for (int i = 0; i < Lines.Num() - 2; i++)
    {
       
        FString line = Lines[i];
        if (line.Contains(STypeMap[SType]))
        {
            int id = -1;
            TArray<FString> tokens;
            line.ParseIntoArray(tokens, TEXT(" "), true);

            id = (int)FCString::Atof(*tokens[1]);

            if (id != SpawnerID) continue;

            if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, 
                            FString::Printf(TEXT(" id = %d,  SpawnerID = %d "), id, SpawnerID));	
            
            FString colsLine = Lines[i + 1];
            FString rowsLine = Lines[i + 2];

            tokens.Empty();
            colsLine.ParseIntoArray(tokens, TEXT(" "), true);
            if (tokens.Num() < 2 || !tokens.Contains("COLS")){
                if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
                            FString::Printf(TEXT(" cols.Num() = %d "), tokens.Num()));	
                continue;
            } 
            ConfigCols = (int)FCString::Atof(*tokens[1]);
            if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
                            FString::Printf(TEXT(" ConfigCols = %d "), ConfigCols));	

            if (ConfigCols < 1) continue;

            tokens.Empty();
            rowsLine.ParseIntoArray(tokens, TEXT(" "), true);

            if (tokens.Num() < 2 || !tokens.Contains("ROWS")){
                if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
                            FString::Printf(TEXT(" raws.Num() = %d "), tokens.Num()));	
                continue;
            } 

            ConfigRows = (int)FCString::Atof(*tokens[1]);
            if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
                            FString::Printf(TEXT(" ConfigRows = %d "), ConfigRows));	
            
            if (ConfigRows < 1) continue;

            /*  one string for config header note */
            if (i > Lines.Num() - 3 - 1 - ConfigRows) continue;

            correctConfig = true;
            for (int j = i + 4; j < i + 4 + ConfigRows; j++)
            {
                tokens.Empty();
                Lines[j].ParseIntoArray(tokens, TEXT(","), true);
                if (tokens.Num() != ConfigCols){
                    if (LogConfig) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
                            FString::Printf(TEXT(" tokens.Num() = %d "), tokens.Num()));	
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

                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
                            FString::Printf(TEXT(" not correctConfig ")));	

                break;
            }

            /* check */
            // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
            //                 FString::Printf(TEXT("cols = %d, rows = %d, AConfig Size = %d"), 
            //                     ConfigCols, ConfigRows, AConfig.Num()));	

            // for (int k = 0; k < AConfig.Num(); k++){
            //     GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
            //         FString::Printf(TEXT("      val[%d] = %f"), k, AConfig[k]));	
            // }

            break;
        }
    }

    return 1;
}

int ASpawner::SetConfig()
{
    if (!RoomConfig) 
        return - 1;
    FString ConfigContent = RoomConfig->GetConfig();
    return SetConfig(ConfigContent);
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
    UE_LOG(LogTemp, Warning, TEXT("ASpawner::BeginPlay"));
    Super::BeginPlay();
    if (PlaceToZero)
        this->SetActorLocation(FVector(0.f));

    InitStorage();
    SetFlows();

    if (RoomConfig)
        RoomConfig->AddSpawner(this);
}

int ASpawner::GetConfigCols(){
    return ConfigCols;
}

void ASpawner::SetEnable(bool enabled)
{
    Enabled = enabled;
}

void ASpawner::SetFlows()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, 
                FString::Printf(TEXT("FlowModels.Num()  %d "), FlowModels.Num()));	

    for (int i = 0; i < FlowModels.Num(); i++)
    {
        AFlowModel * fmodel = FlowModels[i];
        if (!fmodel)
        {
            UE_LOG(LogTemp, Warning, TEXT("SET FLOW: fmodel is NOT valid!"));    
            return;
        }

        fmodel->AddSpawner(this);

        UE_LOG(LogTemp, Warning, TEXT("SET FLOW %d"), fmodel);
        for (auto ptr: InactiveObjects)
        {
            UE_LOG(LogTemp, Warning, TEXT("InactiveObjects SetHostFlow %d"), fmodel);
            ptr->AddHostFlow(fmodel);
        }

        for (auto ptr: ActiveObjects)
        {
            UE_LOG(LogTemp, Warning, TEXT("ActiveObjects SetHostFlow %d"), fmodel);
            ptr->AddHostFlow(fmodel);
        }
    }
    
}

bool ASpawner::TimeToDisactive(TWeakObjectPtr<class AFlowObject> FObject)
{
    bool res = false;
    if (FObject->GetTimeFromRespawn() > SpawnTimeOffset * 1.1f) 
        res = true;

    return res;
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 
}


void ASpawner::InitStorage()
{
    UE_LOG(LogTemp, Warning, TEXT("InitStorage"));

    if (!AFlowObject_Custom_BP)
    {
        UE_LOG(LogTemp, Error, TEXT("AFlowObject_Custom_BP is NOT valid"));
        return;
    }

    for (int i = 0; i < ObjectsAllocatedNum; i++)
    {
        FActorSpawnParameters SpawnParams;
        TWeakObjectPtr<AFlowObject> ptr = 
            GetWorld()->SpawnActor<AFlowObject>(AFlowObject_Custom_BP, FTransform(), SpawnParams);

        // ptr->SetActorHiddenInGame(true);
        // ptr->Init(GetGameTimeSinceCreation(), Flow, this);
        // ptr->Init(GetGameTimeSinceCreation(), NULL, this);
        if (FlowModels.Num() > 0)
            ptr->Init(FlowModels[0]->GetCurrentTime(), NULL, this);
        else 
            ptr->Init(GetGameTimeSinceCreation(), NULL, this);

        ptr->SetActivity(false);
        // ptr->SetActorTickEnabled(false);
        InactiveObjects.Add(ptr);

        // UE_LOG(LogTemp, Error, TEXT("another object spawned"));
    }	
}

void ASpawner::SetVisibility(bool visible){
    Visible = visible;
    for (int i = 0; i < ActiveObjects.Num(); i++){
        ActiveObjects[i]->SetVisibility(visible);
    }

    for (int i = 0; i < InactiveObjects.Num(); i++){
        InactiveObjects[i]->SetVisibility(visible);
    }
}

bool ASpawner::RespawnObject(FTransform Transform, FRotator Spin)
{
    if (InactiveObjects.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("ASpawner ID %d : There is no free object"), SpawnerID);
        return false;
    }

    if (GEngine)
    GEngine->AddOnScreenDebugMessage(-1, 0.09, FColor::Yellow, FString::Printf(TEXT("RespawnObject")));	

    TWeakObjectPtr<AFlowObject> ObjectToSpawn = InactiveObjects[0];
    MoveToActiveArray(0);

       

    ObjectToSpawn->SetLocalTransform(Transform);
    ObjectToSpawn->SetActivity(true);
    ObjectToSpawn->SetPermanentSpin(Spin);
    ObjectToSpawn->SetStartScale(Transform.GetScale3D());

    return true;
}

FTransform ASpawner::GetSpawnerBaseTransform()
{
    if (SpawnerLocationOffset)
    {
        return this->GetActorTransform();
    } else 
    {
        return FTransform();
    }
}

TWeakObjectPtr<AFlowObject> ASpawner::GetInactive()
{
    if (InactiveObjects.Num() > 0)
    {
        return InactiveObjects[0];
    } else 
    {
        return NULL;
        // or create new...
    }
}

void ASpawner::DisactivatePassedObjects()
{
    for (int i = ActiveObjects.Num() - 1; i >= 0; i--)
    {
        if (TimeToDisactive(ActiveObjects[i]))
        {
            // check for order
            ActiveObjects[i]->SetActivity(false);
            ActiveObjects[i]->SetVisibility(false);
            ActiveObjects[i]->SetActorLocation(FVector(1000,10000,10000));
            InactiveObjects.Add(ActiveObjects[i]);
            ActiveObjects.RemoveAt(i);
        }
    }
}

bool ASpawner::MoveToActiveArray(TWeakObjectPtr<AFlowObject> FObject)
{
    ActiveObjects.Add(FObject);
    InactiveObjects.Remove(FObject);
    return true;
}

bool ASpawner::MoveToInactiveArray(TWeakObjectPtr<AFlowObject> FObject)
{
    InactiveObjects.Add(FObject);
    ActiveObjects.Remove(FObject);
    return true;
}

bool ASpawner::MoveToActiveArray(int index)
{
    if (InactiveObjects.IsValidIndex(index))
    {
        ActiveObjects.Add(InactiveObjects[index]);
        InactiveObjects.RemoveAt(index);
        return true;
    } else 
    {
        return false;
    }
}

bool ASpawner::MoveToInactiveArray(int index)
{
    if (ActiveObjects.IsValidIndex(index))
    {
        InactiveObjects.Add(ActiveObjects[index]);
        ActiveObjects.RemoveAt(index);
        return true;
    } else 
    {
        return false;
    }
}
void ASpawner::Stop(){
    Active = false;

    for (auto ptr: ActiveObjects){
        ptr->Disactivate();
        ptr->SetVisibility(false);
    }
}

void ASpawner::Restart(){}