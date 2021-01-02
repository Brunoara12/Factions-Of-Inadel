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
			WorldSectors = FWorldSectors(9);
			FString Ext = "save";
			FString FileName = "";
			FFileManagerGeneric::Get().FindFiles(ArrayOfSavedSectors, *Directory, *Ext);
			// TODO Load ALL save sector data
			for (int i = 0; i < ArrayOfSavedSectors.Num(); i++)
			{
				SaveFile = Directory + "\\" + ArrayOfSavedSectors[i];

				// Create new Sector
				SpawnedActor = World->SpawnActor<AGridSetup>(AGridSetup::StaticClass(), FTransform());

				// Load Sector Data from SaveFile into WorldSector
				PC->LoadGameDataFromFile(SaveFile, SpawnedActor->MapSector, SectorCoord);
				V_LOGM("Loaded Sector: %s", *SectorCoord.ToString());

				// Places Created Sector Grid into World Sector
				WorldSectors.GetZ(SectorCoord.Z).GetX(SectorCoord.X).GetY(SectorCoord.Y) = SpawnedActor;

				// Spawns all Cells in sector
				WorldSectors.GetZ(SectorCoord.Z).GetX(SectorCoord.X).GetY(SectorCoord.Y)->SetupGridSector(SectorCoord);	
			}
		}
	}
}
