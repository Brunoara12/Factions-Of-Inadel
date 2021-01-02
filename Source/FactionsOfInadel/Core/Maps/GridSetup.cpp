// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSetup.h"
#include "../HelperFiles/DefinedDebugHelpers.h"
#include "GridBase.h"
#include "RockWalls.h"
#include "../GameModes/FactionsOfInadelPlayerController.h"

// Sets default values
AGridSetup::AGridSetup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SectorLocation = FVector(0, 0, 0);
	CellSize = 100;
	MapSector = FMapSector(3);

	SectorSize.Z = MapSector.Layers.Num();
	SectorSize.X= MapSector.Layers[0].Rows.Num();
	SectorSize.Y = MapSector.Layers[0].Rows[0].Columns.Num();
}

void AGridSetup::SetSectorLocation(FVector SectorCoords)
{
	SectorLocation = SectorCoords;
}

void AGridSetup::SetupGridSector(FVector SectorCoords)
{
	SetSectorLocation(SectorCoords);
	// Sets Grid up
	for (int32 z = 0; z < SectorSize.Z; z++)
	{
		for (int32 x = 0; x < SectorSize.X; x++)
		{
			for (int32 y = 0; y < SectorSize.Y; y++)
			{
				
				SpawnCell(FVector(x,y,z));
			}
		}
	}
}

void AGridSetup::SpawnCell(FVector CellToSpawn)
{
	int32 CurrentGridPosZ = (CellToSpawn.Z + (SectorLocation.Z * SectorSize.Z)) * CellSize;
	int32 CurrentGridPosX = (CellToSpawn.X + (SectorLocation.X * SectorSize.X)) * CellSize;
	int32 CurrentGridPosY = (CellToSpawn.Y + (SectorLocation.Y * SectorSize.Y)) * CellSize;


	FVector Temp = FVector(CellSize/2 + CurrentGridPosX, // X of Actor is half of its size
		CellSize/2 + CurrentGridPosY, // Y of Actor is half of its size
		CurrentGridPosZ); // Z of Actor is 0

	FTransform TempTran = FTransform(Temp + GetActorLocation());
	TempTran.SetRotation(GetActorRotation().Quaternion());


	AGridBase* SpawnCell = NULL;
	// TEMP Determinant of what Letter corresponds to what Mesh
	switch (MapSector.Layers[CellToSpawn.Z].Rows[CellToSpawn.X].Columns[CellToSpawn.Y])
	{
	case 0:
		//SpawnCell = GetWorld()->SpawnActor<AGridBase>(AGridBase::StaticClass(), TempTran);
		//SpawnCell->ChangeToAir();
		break;
	case 1:
		SpawnCell = GetWorld()->SpawnActor<ARockWalls>(ARockWalls::StaticClass(), TempTran);
		break;
	case 2:
		SpawnCell = GetWorld()->SpawnActor<AGridBase>(AGridBase::StaticClass(), TempTran);
		SpawnCell->ChangeToGrassMaterial();
		break;
	default:
		SpawnCell = GetWorld()->SpawnActor<AGridBase>(AGridBase::StaticClass(), TempTran);
		break;


	}
	if (SpawnCell)
	{
		SpawnCell->SetCellCoord(FVector(CellToSpawn.X, CellToSpawn.Y, CellToSpawn.Z), SectorLocation);
	}
}

void AGridSetup::PrintSector(FMapSector MapSector)
{
	// Display Sector
	FString Col = "";
	for (int32 z = 0; z < MapSector.Layers.Num(); z++)
	{
		V_LOG(" ");
		for (int32 x = 0; x < MapSector.Layers[0].Rows.Num(); x++)
		{
			for (int32 y = 0; y < MapSector.Layers[0].Rows[0].Columns.Num(); y++)
			{
				Col.Append(FString::FromInt(MapSector.Layers[z].Rows[x].Columns[y]));

			}
			V_LOG(*Col);
			Col.Empty();
		}
	}
}

// Called when the game starts or when spawned
void AGridSetup::BeginPlay()
{
	Super::BeginPlay();


	// Sets up for Grid

	FString LevelMap = ReadFile("LevelMap.txt");

	/*
	// Gets rid of nextline and return characters
	int idx = 0;
	bool bOnce = false;
	for (auto& LevelCell : LevelMap)
	{
		if (LevelCell == '\r')
		{
			//LevelMap.RemoveAt(idx, 2);
			GridY++;
			//V_LOG("/r");
			bOnce = true;
		}
		else if (bOnce != true)
		{
			GridX++;
		}
		idx++;
	}*/
	

	// MapLayers[0] will be the first layer of the map made up of rows and columns
	//MapLayers.Add(FMapLayer(GridX - 1, GridY - 1));

	
}

// Returns FString of all the contents in FileName.txt
FString AGridSetup::ReadFile(FString FileName)
{
	//Read file ini [project]/Source/FactionsOfInadel/Core/Maps/ 
	//you can change with other location
	FString Source = FPaths::GameSourceDir();
	FString directory = Source + "FactionsOfInadel/Core/Maps";
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (file.CreateDirectory(*directory)) {
		FString myFile = directory + "/" + FileName;
		FFileHelper::LoadFileToString(result, *myFile);
	}
	return result;
}

// Called every frame
void AGridSetup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

