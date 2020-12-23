// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBase.h"
#include "../HelperFiles/DefinedDebugHelpers.h"

// Sets default values
AGridBase::AGridBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->HInstMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
		TEXT("HierarchicalInstancedStaticMesh"));
	this->SceneComp = CreateDefaultSubobject<USceneComponent>(
		TEXT("Scene Component"));
	this->BoxComp = CreateDefaultSubobject<UBoxComponent>(
		TEXT("Box Component"));

	//this->HInstMesh = CreateDefaultSubobject<UStaticMeshComponent>(
	//	TEXT("HierarchicalInstancedStaticMesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>FoundMesh(
		TEXT("/Game/FoI/Maps/Assets/SM_GridBase_100x100.SM_GridBase_100x100"));
	if (FoundMesh.Succeeded())
	{
		HInstMesh->SetStaticMesh(FoundMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>FoundMaterial(
		TEXT("/Game/FoI/Maps/Assets/M_GridBase.M_GridBase"));
	static ConstructorHelpers::FObjectFinder<UMaterial>FoundGrassMaterial(
		TEXT("/Game/FoI/Maps/Assets/M_Grass.M_Grass"));
	static ConstructorHelpers::FObjectFinder<UMaterial>FoundAirMaterial(
		TEXT("/Game/FoI/Maps/Assets/M_Transparent.M_Transparent"));
	if (FoundMaterial.Succeeded())
	{
		Material = FoundMaterial.Object;
	}
	if (FoundGrassMaterial.Succeeded())
	{
		GrassMaterial = FoundGrassMaterial.Object;
	}
	if (FoundGrassMaterial.Succeeded())
	{
		AirMaterial= FoundAirMaterial.Object;
	}

	this->GridMeshMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	this->HInstMesh->SetMaterial(0, GridMeshMaterial);

	RootComponent = SceneComp;
	//SetRootComponent(SceneComp);
	HInstMesh->SetupAttachment(SceneComp);
	BoxComp->SetupAttachment(SceneComp);

	BoxComp->InitBoxExtent(FVector(50, 50, 1));
	
	BoxComp->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void AGridBase::BeginPlay()
{
	Super::BeginPlay();

	RegisterAllComponents();
	this->HInstMesh->ClearInstances();
	this->HInstMesh->AddInstance(FTransform(FVector(0.0, 0.0, 0.0)));

	//this->HInstMesh->MarkRenderStateDirty();
}

// Called every frame
void AGridBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridBase::SetCellCoord(FVector CellCoords, FVector SectorCoords)
{
	CellCoord = CellCoords;
	SectorCoord = SectorCoords;
}

void AGridBase::ChangeToGrassMaterial()
{
	this->GridMeshMaterial = UMaterialInstanceDynamic::Create(GrassMaterial, NULL);
	this->HInstMesh->SetMaterial(0, GridMeshMaterial);
}

void AGridBase::ChangeToAir()
{
	this->GridMeshMaterial = UMaterialInstanceDynamic::Create(AirMaterial, NULL);
	this->HInstMesh->SetMaterial(0, GridMeshMaterial);
}

