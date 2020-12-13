// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSetup.generated.h"

class AGridBase;
UCLASS()
class FACTIONSOFINADEL_API AGridSetup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridSetup();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridX;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridY;

	// Grid Size of Cell Currently 96x96 units
	int32 GridSize;

	// Temp Array of all Meshes inside LevelMap
	TArray<AGridBase*> GridMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Returns FString of all the contents in FileName.txt
	FString ReadFile(FString FileName);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
