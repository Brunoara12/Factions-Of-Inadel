// Fill out your copyright notice in the Description page of Project Settings.


#include "RockWalls.h"
#include "../HelperFiles/DefinedDebugHelpers.h"


ARockWalls::ARockWalls()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*this->HInstMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
		TEXT("HierarchicalInstancedStaticMesh"));
	this->SceneComp = CreateDefaultSubobject<USceneComponent>(
		TEXT("Scene Component"));
	this->BoxComp = CreateDefaultSubobject<UBoxComponent>(
		TEXT("Box Component"));*/

		//this->HInstMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		//	TEXT("HierarchicalInstancedStaticMesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>FoundMesh(
		TEXT("/Game/FoI/Maps/Assets/SM_TestCube_96x96.SM_TestCube_96x96"));
	if (FoundMesh.Succeeded())
	{
		HInstMesh->SetStaticMesh(FoundMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>FoundMaterial(
		TEXT("/Game/StarterContent/Materials/M_Rock_Sandstone.M_Rock_Sandstone"));
	if (FoundMaterial.Succeeded())
	{
		Material = FoundMaterial.Object;
		//V_LOG("It Worked!");
	}

	this->GridMeshMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	this->HInstMesh->SetMaterial(0, GridMeshMaterial);

	RootComponent = SceneComp;
	HInstMesh->SetupAttachment(SceneComp);
	BoxComp->SetupAttachment(SceneComp);

	BoxComp->SetBoxExtent(FVector(48, 48, 2));
	BoxComp->SetVisibility(true);
	BoxComp->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void ARockWalls::BeginPlay()
{
	Super::BeginPlay();

	RegisterAllComponents();
	this->HInstMesh->ClearInstances();
	this->HInstMesh->AddInstance(FTransform(FVector(0.0, 0.0, 0.0)));

	//this->HInstMesh->MarkRenderStateDirty();
}