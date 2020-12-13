// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSetup.h"
#include "../HelperFiles/DefinedDebugHelpers.h"
#include "GridBase.h"
#include "RockWalls.h"

// Sets default values
AGridSetup::AGridSetup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridSize = 96;
	GridX = 0;
	GridY = 0;
}

// Called when the game starts or when spawned
void AGridSetup::BeginPlay()
{
	Super::BeginPlay();

	GridX++;
	GridY++;
	// Sets up for Grid
	int32 CenterPointX = (GridX * GridSize) / 4;
	int32 CenterPointY = (GridY * GridSize) / 4;


	FString LevelMap = ReadFile("LevelMap.txt");

	// Gets rid of nextline and return characters
	int idx = 0;
	bool bOnce = false;
	for (auto& LevelCell : LevelMap)
	{
		if (LevelCell == '\r')
		{
			LevelMap.RemoveAt(idx, 2);
			GridY++;
			//V_LOG("/r");
			bOnce = true;
		}
		else if (bOnce != true)
		{
			GridX++;
		}
		idx++;
	}
	GridY++;

	/*V_LOGI("LevelMap Size", LevelMap.Len());
	V_LOGI("GridX: ", GridX);
	V_LOGI("GridY: ", GridY);*/

	// Sets Grid up
	int HalfOfGridSize = GridSize / 2;
	for (int i = 1; i < GridY; i++)
	{
		for (int j = 1; j < GridX; j++)
		{
			int CurrentGridPosX = i * GridSize;
			int CurrentGridPosY = j * GridSize;
			//V_LOGM("i:%d, CurrPosX:%d", i, CurrentGridPosX);
			//V_LOGM("j:%d, CurrPosY:%d", j, CurrentGridPosY);

			FVector Temp = FVector(CenterPointX - CurrentGridPosX - HalfOfGridSize,
				CenterPointY + CurrentGridPosY + HalfOfGridSize,
				0.0);

			FTransform TempTran = FTransform(Temp + GetActorLocation());
			TempTran.SetRotation(GetActorRotation().Quaternion());
			//V_LOGM("Rot: %s", *GetActorRotation().Quaternion().ToString());
			AGridBase* NewActor;

			// TEMP Determinant of what Letter corresponds to what Mesh
			if (LevelMap[(j - 1) + ((GridX - 1) * (i - 1))] == 'R')
			{
				NewActor = GetWorld()->SpawnActor<ARockWalls>(ARockWalls::StaticClass(), TempTran);
			}
			else
			{
				NewActor = GetWorld()->SpawnActor<AGridBase>(AGridBase::StaticClass(), TempTran);
			}

			if (NewActor)
			{
				GridMesh.Add(NewActor);
			}
		}
	}
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

