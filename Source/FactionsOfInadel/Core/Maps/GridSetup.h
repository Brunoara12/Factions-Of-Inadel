// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridBase.h"

#include "GridSetup.generated.h"

USTRUCT()
struct FMapRow {
	GENERATED_BODY()

		UPROPERTY()
		TArray<int8> Columns;

	FORCEINLINE int8& operator[] (int32 j)
	{
		return Columns[j];
	}

	void SetGridBase(int8 Block, int32 y)
	{
		Columns[y] = Block;
	}

	void AddNewColumn()
	{
		Columns.Add(0);
	}

	FMapRow()
	{

	}
};

USTRUCT() 
struct FMapLayer {
	GENERATED_BODY()

		UPROPERTY()
		TArray<FMapRow> Rows;

	FORCEINLINE FMapRow& operator[] (int32 i)
	{
		return Rows[i];
	}

	void AddNewRow()
	{
		Rows.Add(FMapRow());
	}

	void AddUninitialized(const int32 RowCount, const int32 ColCount)
	{
		Clear();
		//Add Rows
		for (int32 v = 0; v < RowCount; v++)
		{
			AddNewRow();
		}

		//Add Columns
		for (int32 v = 0; v < RowCount; v++)
		{
			for (int32 b = 0; b < ColCount; b++)
			{
				Rows[v].AddNewColumn();
			}
		}
	}

	void Clear()
	{
		if (Rows.Num() <= 0) return;
		//~~~~~~~~~~~~~~~

		//Destroy any Actors
		const int32 RowTotal = Rows.Num();
		const int32 ColTotal = Rows[0].Columns.Num();

		//Empty
		for (int32 v = 0; v < Rows.Num(); v++)
		{
			Rows[v].Columns.Empty();
		}
		Rows.Empty();
	}

	TTuple<int32, int32> NumLayerDimensions()
	{
		return TTuple<int32, int32>(Rows.Num(), Rows[0].Columns.Num());
	}

	//default properties
	FMapLayer()
	{

	}

	FMapLayer(int32 Rows, int32 Cols) 
	{
		AddUninitialized(Rows, Cols);
	}
};

USTRUCT()
struct FMapSector {
	GENERATED_BODY()

	UPROPERTY()
	TArray<FMapLayer> Layers;

	FORCEINLINE FMapLayer& operator[] (int32 i)
	{
		return Layers[i];
	}

	void AddNewLayer()
	{
		Layers.Add(FMapLayer());
	}

	void AddUnitialized(int32 LayerCount, int32 RowCount, int32 ColCount)
	{
		Clear();
		// Add Layers
		for (int32 l = 0; l < LayerCount; l++)
		{
			AddNewLayer();
		}

		// Add Rows
		for (int32 l = 0; l < LayerCount; l++)
		{
			for (int32 r = 0; r < RowCount; r++)
			{
				Layers[l].AddNewRow();
			}
		}

		// Add Rows
		for (int32 l = 0; l < LayerCount; l++)
		{
			for (int32 r = 0; r < RowCount; r++)
			{
				for (int32 c = 0; c < ColCount; c++)
				{
					Layers[l].Rows[r].AddNewColumn();
				}
			}
		}
	}

	void Clear()
	{
		if (Layers.Num() <= 0) return;
		//~~~~~~~~~~~~~~~

		//Destroy any Actors
		const int32 LayerTotal = Layers.Num();
		const int32 RowTotal = Layers[0].Rows.Num();
		const int32 ColTotal = Layers[0].Rows[0].Columns.Num();


		//Empty
		for (int32 l = 0; l < LayerTotal; l++)
		{
			for (int32 r = 0; r < RowTotal; r++)
			{
				Layers[l].Rows[r].Columns.Empty();
			}
		}
		for (int32 l = 0; l < LayerTotal; l++)
		{
			Layers[l].Rows.Empty();
		}

		Layers.Empty();
	}

	FMapSector()
	{
		AddUnitialized(3, 3, 3);
	}

	FMapSector(int32 size)
	{
		AddUnitialized(size, size, size);
	}

	FMapSector(int32 Layers, int32 Rows, int32 Cols)
	{
		AddUnitialized(Layers, Rows, Cols);
	}
};

class AGridBase;
UCLASS()
class FACTIONSOFINADEL_API AGridSetup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridSetup();


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector SectorLocation;

	FVector SectorSize;
	// Grid Size of Cell Currently 100x100 units
	int32 CellSize;

	// 3D Array of all Meshes inside LevelMap
	UPROPERTY()
	FMapSector MapSector;

	void SetSectorLocation(FVector SectorCoords);

	// Setup and Spawn Cells for Map
	void SetupGridSector(FVector SectorCoords);

	// Spawn Cell into Map
	void SpawnCell(FVector CellToSpawn);

	// Log MapSector Visually
	void PrintSector(FMapSector MapSector);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Returns FString of all the contents in FileName.txt
	FString ReadFile(FString FileName);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
