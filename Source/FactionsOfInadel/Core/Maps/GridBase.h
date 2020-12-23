// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GridBase.generated.h"

UCLASS()
class FACTIONSOFINADEL_API AGridBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridBase();


	UMaterial* Material;
	UMaterial* GrassMaterial;
	UMaterial* AirMaterial;

	UPROPERTY(EditAnywhere)
		USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
		UMaterialInstanceDynamic* GridMeshMaterial;

	AActor* AssignedObject;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent* HInstMesh;
	//UStaticMeshComponent* HInstMesh;

	FVector CellCoord;
	FVector SectorCoord;

	bool bGridUsable;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetCellCoord(FVector CellCoords, FVector SectorCoords);

	void ChangeToGrassMaterial();

	void ChangeToAir();
};
