// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSectorSetup.h"
#include "../HelperFiles/DefinedDebugHelpers.h"
#include "HAL/FileManagerGeneric.h"
#include "Misc/Paths.h"
#include "../GameModes/FactionsOfInadelPlayerController.h"


// Sets default values
AGridSectorSetup::AGridSectorSetup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void AGridSectorSetup::SpawnSectorGrid(FVector SectorCoord)
{
}

void AGridSectorSetup::BeginPlay()
{
	FString Directory = FPaths::ProjectDir() + "MapData";
	UWorld* World = GetWorld();
	AFactionsOfInadelPlayerController* PC = NULL;
	if (World)
	{
		PC = Cast<AFactionsOfInadelPlayerController>(World->GetFirstPlayerController());
		FString SaveFile;
		FVector SectorCoord;
		AGridSetup* SpawnedActor;
		if (FPaths::DirectoryExists(Directory))
		{
			// Set to -4 , 4 for the time being. total of 9x9x9 - 729 Sectors
			WorldSectors = FWorldSectors();
			FString Ext = "save";
			FString FileName = "";
			FFileManagerGeneric::Get().FindFiles(ArrayOfSavedSectors, *Directory, *Ext);
			// TODO Load ALL save sector data
			for (int i = 0; i < ArrayOfSavedSectors.Num(); i++)
			{
				SaveFile = Directory + "\\" + ArrayOfSavedSectors[i];

				//FPaths::Split(SaveFile, Directory, FileName, Ext);

				SpawnedActor = World->SpawnActor<AGridSetup>(AGridSetup::StaticClass(), FTransform());

				// Load Sector Data from SaveFile into WorldSector
				PC->LoadGameDataFromFile(SaveFile, SpawnedActor->MapSector, SectorCoord);
				


				// Spawn Sector Grid into World Sector
				WorldSectors.GetZ(SectorCoord.Z).GetX(SectorCoord.X).GetY(SectorCoord.Y) = SpawnedActor;


				// DELETE{
				SpawnSectorGrid(FVector());

				// Spawns all Cells in sector
				WorldSectors.GetZ(0).GetX(0).GetY(0)->SetupGridSector(SectorCoord);
				
		
				V_LOGM("%d, x: %d, y: %d, z: %d", WorldSectors.GetZ(0).GetX(0).GetY(0)->MapSector.Layers[0].Rows[0].Columns[0], 
					WorldSectors.GetZ(0).GetX(0).GetY(0)->SectorLocation.X,
					WorldSectors.GetZ(0).GetX(0).GetY(0)->SectorLocation.Y,
					WorldSectors.GetZ(0).GetX(0).GetY(0)->SectorLocation.Z);
				/*for (int32 l = 0; l < 3; l++)
				{
					for (int32 r = 0; r < 3; r++)
					{
						for (int32 c = 0; c < 3; c++)
						{
							V_LOGM("Map Sector: %d", WorldSectors.SectorsZ[0].SectorsX[0].SectorsY[i].Layers[l].Rows[r].Columns[c]);
						}
					}
				}*/

			}
		}
	}
}
