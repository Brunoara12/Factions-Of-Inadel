// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridSetup.h"
#include "GridSectorSetup.generated.h"


//USTRUCT()
//struct FWorldSectorsY {
//	GENERATED_BODY()
//
//		UPROPERTY()
//		TArray<FMapSector> SectorsY;
//
//	int8 OFFSET = 4;
//
//	FMapSector& GetY(int32 i)
//	{
//		return SectorsY[i + OFFSET];
//	}
//
//	void AddNewMapSector()
//	{
//		SectorsY.Add(FMapSector());
//	}
//
//	FWorldSectorsY()
//	{
//	}
//};

USTRUCT()
struct FWorldSectorsY {
	GENERATED_BODY()

		UPROPERTY()
		TArray<AGridSetup *> SectorsY;

	int8 OFFSET = 4;

	AGridSetup* &GetY(int32 i)
	{
		return SectorsY[i + OFFSET];
	}

	void AddNewMapSector()
	{
		SectorsY.Add(NULL);
	}

	FWorldSectorsY()
	{
	}
};

USTRUCT()
struct FWorldSectorsX {
	GENERATED_BODY()

		UPROPERTY()
		TArray<FWorldSectorsY> SectorsX;

	int8 OFFSET = 4;

	FWorldSectorsY& GetX(int32 i)
	{
 		return SectorsX[i + OFFSET];
	}

	void AddNewSectorY()
	{
		SectorsX.Add(FWorldSectorsY());
	}

	FWorldSectorsX()
	{

	}
};

USTRUCT()
struct FWorldSectors {
	GENERATED_BODY()

		UPROPERTY()
		TArray<FWorldSectorsX> SectorsZ;

	int32 SectorDimension;
	int8 OFFSET = 4;

	FWorldSectorsX& GetZ(int32 i)
	{
		return SectorsZ[i + OFFSET];
	}

	void AddNewSectorX()
	{
		SectorsZ.Add(FWorldSectorsX());
	}

	void AddUnitialized(int8 SectorZCount, int8 SectorXCount, int8 SectorYCount)
	{
		Clear();
		// Add Z
		for (int32 z = 0; z < SectorZCount; z++)
		{
			AddNewSectorX();
		}

		// Add X
		for (int32 z = 0; z < SectorZCount; z++)
		{
			for (int32 x = 0; x < SectorXCount; x++)
			{
				SectorsZ[z].AddNewSectorY();
			}
		}

		// Add Y
		for (int32 z = 0; z < SectorZCount; z++)
		{
			for (int32 x = 0; x < SectorXCount; x++)
			{
				for (int32 y = 0; y < SectorYCount; y++)
				{
					SectorsZ[z].SectorsX[x].AddNewMapSector();
				}
			}
		}
	}

	void Clear()
	{
		if (SectorsZ.Num() <= 0) return;
		//~~~~~~~~~~~~~~~

		//Destroy any Actors
		const int32 SectorZTotal = SectorsZ.Num();
		const int32 SectorXTotal = SectorsZ[0].SectorsX.Num();
		const int32 SectorYTotal = SectorsZ[0].SectorsX[0].SectorsY.Num();


		for (int32 z = 0; z < SectorZTotal; z++)
		{
			for (int32 x = 0; x < SectorXTotal; x++)
			{
				for (int32 y = 0; y < SectorYTotal; y++)
				{
					SectorsZ[z].SectorsX[x].SectorsY[y]->Destroy();
				}
			}
		}


		//Empty
		for (int32 z = 0; z < SectorZTotal; z++)
		{
			for (int32 x = 0; x < SectorXTotal; x++)
			{
				SectorsZ[z].SectorsX[x].SectorsY.Empty();
			}
		}
		for (int32 z = 0; z < SectorZTotal; z++)
		{
			SectorsZ[z].SectorsX.Empty();
		}

		SectorsZ.Empty();
	}

	// ex. SectorDimension = 9 then it's 9x9x9 = 729 Sector in World
	FWorldSectors(int32 SectorDimension) 
	{
		AddUnitialized(SectorDimension, SectorDimension, SectorDimension);
		this->SectorDimension = SectorDimension;
	}

	FWorldSectors()
	{

	}
};



/**
 * 
 */
UCLASS()
class FACTIONSOFINADEL_API AGridSectorSetup : public AGridSetup
{
	GENERATED_BODY()
	
public:

	AGridSectorSetup();



	FWorldSectors WorldSectors;

	// File Directories to all saved sector files
	TArray<FString> ArrayOfSavedSectors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
