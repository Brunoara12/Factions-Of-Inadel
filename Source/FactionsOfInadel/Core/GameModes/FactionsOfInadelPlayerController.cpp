// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactionsOfInadelPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "../MainCharacter/FactionsOfInadelCharacter.h"
#include "../HelperFiles/DefinedDebugHelpers.h"

#include "Engine/World.h"

AFactionsOfInadelPlayerController::AFactionsOfInadelPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AFactionsOfInadelPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AFactionsOfInadelPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AFactionsOfInadelPlayerController::OnLeftClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AFactionsOfInadelPlayerController::OnRightClick);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFactionsOfInadelPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFactionsOfInadelPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AFactionsOfInadelPlayerController::OnResetVR);


	// Toggle Between Edit Mode and Play Mode
	InputComponent->BindAction("EditWorld", IE_Pressed, this, &AFactionsOfInadelPlayerController::OnEditMode);

	// Save while in Edit Mode
	InputComponent->BindAction("SaveWorldData", IE_Pressed, this, &AFactionsOfInadelPlayerController::OnSaveMapData);
}

void AFactionsOfInadelPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFactionsOfInadelPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AFactionsOfInadelCharacter* MyPawn = Cast<AFactionsOfInadelCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AFactionsOfInadelPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AFactionsOfInadelPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AFactionsOfInadelPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AFactionsOfInadelPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AFactionsOfInadelPlayerController::OnLeftClick()
{
	FHitResult Hit;

	if (bInEditMode)
	{
		// Trace to see what is under the mouse cursor
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something
			AActor* HitActor = Hit.GetActor();
			if (HitActor->IsA<AGridBase>())
			{
				UWorld* World = GetWorld();
				SCREENMSGC("Hit a Cell", Blue);
				DEBUGLC(Hit.ImpactPoint, (Hit.ImpactNormal * 200) + Hit.ImpactPoint, Yellow, 3);
				AGridBase* CellActor = Cast<AGridBase>(HitActor);
				FVector CellCoord = CellActor->CellCoord;
				FVector SectorCoord = CellActor->SectorCoord;
				SCREENMSG(CellCoord.ToString());
				SCREENMSG(SectorCoord.ToString());
				SCREENMSGC(Hit.ImpactNormal.ToString(), Red);
				FVector NewCellCoord = CellCoord + Hit.ImpactNormal;
				AGridSetup* GridSetup = SectorSetup->WorldSectors.GetZ(SectorCoord.Z).GetX(SectorCoord.X).GetY(SectorCoord.Y);
				
				int32 SectorSize = GridSetup->SectorSize.Z;
				int32 CellSize = GridSetup->CellSize;

				FVector NewSectorCoord = SectorCoord;
				if (NewCellCoord.Z >= SectorSize)
				{
					NewSectorCoord.Z =  SectorCoord.Z + 1;
					NewCellCoord.Z = 0;
					GridSetup = SectorSetup->WorldSectors.GetZ(NewSectorCoord.Z).GetX(NewSectorCoord.X).GetY(NewSectorCoord.Y);
					if (!GridSetup){
						// Spawn in New Sector Above Current One
						GridSetup =	World->SpawnActor<AGridSetup>(AGridSetup::StaticClass(), 
							FTransform(FVector(NewSectorCoord.X,NewSectorCoord.Y,NewSectorCoord.Z + (SectorSize * CellSize ))));
						
					}
				}
				else if (NewCellCoord.Z < 0)
				{
					NewSectorCoord.Z = SectorCoord.Z - 1;
					NewCellCoord.Z = SectorSize - 1;
					GridSetup = SectorSetup->WorldSectors.GetZ(NewSectorCoord.Z).GetX(NewSectorCoord.X).GetY(NewSectorCoord.Y);
					if (!GridSetup) {
						// Spawn in New Sector Below Current One
						GridSetup = World->SpawnActor<AGridSetup>(AGridSetup::StaticClass(),
							FTransform(FVector(NewSectorCoord.X, NewSectorCoord.Y, NewSectorCoord.Z - (SectorSize * CellSize))));

					}
				}
				else if (NewCellCoord.X >= SectorSize)
				{
					NewSectorCoord.X = SectorCoord.X + 1;
					NewCellCoord.X = 0;
					GridSetup = SectorSetup->WorldSectors.GetZ(NewSectorCoord.Z).GetX(NewSectorCoord.X).GetY(NewSectorCoord.Y);
					if (!GridSetup) {
						// Spawn in New Sector Towards Positive X Current One
						GridSetup = World->SpawnActor<AGridSetup>(AGridSetup::StaticClass(),
							FTransform(FVector(NewSectorCoord.X + (SectorSize * CellSize), NewSectorCoord.Y, NewSectorCoord.Z)));

					}
				}
				else if (NewCellCoord.X < 0)
				{
					NewSectorCoord.X = SectorCoord.X - 1;
					NewCellCoord.X = SectorSize - 1;
					GridSetup = SectorSetup->WorldSectors.GetZ(NewSectorCoord.Z).GetX(NewSectorCoord.X).GetY(NewSectorCoord.Y);
					if (!GridSetup) {
						// Spawn in New Sector Towards Negative X Current One
						GridSetup = World->SpawnActor<AGridSetup>(AGridSetup::StaticClass(),
							FTransform(FVector(NewSectorCoord.X - (SectorSize * CellSize), NewSectorCoord.Y, NewSectorCoord.Z)));
					}
				}
				else if (NewCellCoord.Y >= SectorSize)
				{
					NewSectorCoord.Y = SectorCoord.Y + 1;
					NewCellCoord.Y = 0;
					GridSetup = SectorSetup->WorldSectors.GetZ(NewSectorCoord.Z).GetX(NewSectorCoord.X).GetY(NewSectorCoord.Y);
					if (!GridSetup) {
						// Spawn in New Sector Towards Positive Y Current One
						GridSetup = World->SpawnActor<AGridSetup>(AGridSetup::StaticClass(),
							FTransform(FVector(NewSectorCoord.X, NewSectorCoord.Y + (SectorSize * CellSize), NewSectorCoord.Z)));

					}
				}
				else if (NewCellCoord.Y < 0)
				{
					NewSectorCoord.Y = SectorCoord.Y - 1;
					NewCellCoord.Y = SectorSize - 1;
					GridSetup = SectorSetup->WorldSectors.GetZ(NewSectorCoord.Z).GetX(NewSectorCoord.X).GetY(NewSectorCoord.Y);
					if (!GridSetup) {
						// Spawn in New Sector Towards Negative Y Current One
						GridSetup = World->SpawnActor<AGridSetup>(AGridSetup::StaticClass(),
							FTransform(FVector(NewSectorCoord.X, NewSectorCoord.Y - (SectorSize * CellSize), NewSectorCoord.Z)));

					}
				}
				GridSetup->SetSectorLocation(NewSectorCoord);
				SectorSetup->WorldSectors.GetZ(NewSectorCoord.Z).GetX(NewSectorCoord.X).GetY(NewSectorCoord.Y) = GridSetup;

				GridSetup = SectorSetup->WorldSectors.GetZ(NewSectorCoord.Z).GetX(NewSectorCoord.X).GetY(NewSectorCoord.Y);
				GridSetup->MapSector.Layers[NewCellCoord.Z].Rows[NewCellCoord.X].Columns[NewCellCoord.Y] = 1;
 				GridSetup->SpawnCell(FVector(NewCellCoord.X , 
					NewCellCoord.Y,
					NewCellCoord.Z));
				V_LOGM("Spawned Cell at Sector: %s  CellCoord at : %s", *NewSectorCoord.ToString(), *NewCellCoord.ToString());

			}
			else
			{
				SCREENMSGC("Did not hit a Cell", Yellow);
				DEBUGPC(FVector(0, 0, 0), Red, 10);
				DEBUGPC(Hit.ImpactNormal, Red, 10);
				DEBUGPC(Hit.Normal, Green, 10);
			}
		}
	}
}

void AFactionsOfInadelPlayerController::OnRightClick()
{
}

void AFactionsOfInadelPlayerController::OnSaveMapData()
{
	if (bInEditMode)
	{
		FBufferArchive ToBinary;

		

		// TODO Create Save for all Sectors
		AGridSetup* GridSetup = SectorSetup->WorldSectors.GetZ(0).GetX(0).GetY(0); // Sector 0,0,0 for now
		FMapSector CurrentMapSector = GridSetup->MapSector;
		

		FVector SectorCoord = GridSetup->SectorLocation;
		

		bool SaveSuccess = SaveGameDataToFile("C:\\Users\\Bruno\\UnrealProjects_4_26\\FactionsOfInadel\\MapData\\" + SectorCoord.ToCompactString() + ".save", 
			ToBinary, 
			CurrentMapSector, 
			SectorCoord);

		if (SaveSuccess)
		{
			SCREENMSG("Save Successful!");
		}
		else
		{
			SCREENMSGC("Save Successful!", Yellow);
		}
	}
	else
	{
		SCREENMSGC("Not in Edit Mode", Red);
	}
}

void AFactionsOfInadelPlayerController::OnLoadMapData()
{
}

void AFactionsOfInadelPlayerController::OnEditMode()
{
	bInEditMode = !bInEditMode;
	if (bInEditMode)
	{
		SCREENMSGC("Currently in Edit Mode", Red);
	}
	else
	{
		SCREENMSG("Currently Out of Edit Mode");
	}
}

void AFactionsOfInadelPlayerController::MoveLeft()
{
}

void AFactionsOfInadelPlayerController::MoveRight()
{
}

void AFactionsOfInadelPlayerController::MoveForward()
{
}

void AFactionsOfInadelPlayerController::MoveBack()
{
}

bool AFactionsOfInadelPlayerController::SaveGameDataToFile(const FString& FullFilePath, FBufferArchive& ToBinary, FMapSector& MapSector, FVector& SectorCoord)
{
	//note that the supplied FString must be the entire Filepath
		//  if writing it out yourself in C++ make sure to use the \\
	    //  for example:

		//  FString SavePath = "C:\\MyProject\\MySaveDir\\mysavefile.save";

		//Step 1: Variable Data -> Binary

		//following along from above examples
	

	SaveLoadData(ToBinary, MapSector, SectorCoord);
	//presumed to be global var data, 
	//could pass in the data too if you preferred

	//No Data
	if (ToBinary.Num() <= 0) return false;
	//~

	//Step 2: Binary to Hard Disk
	if (FFileHelper::SaveArrayToFile(ToBinary, *FullFilePath))
	{
		// Free Binary Array    
		ToBinary.FlushCache();
		ToBinary.Empty();

		V_LOG("Save Success!");
		return true;
	}

	// Free Binary Array    
	ToBinary.FlushCache();
	ToBinary.Empty();

	V_LOG("File Could Not Be Saved!");

	return false;
}

bool AFactionsOfInadelPlayerController::LoadGameDataFromFile(const FString& FullFilePath, FMapSector& MapSector, FVector& SectorCoord)
{
	//Load the data array,
		//  you do not need to pre-initialize this array,
		//      UE4 C++ is awesome and fills it 
		//      with whatever contents of file are, 
		//      and however many bytes that is
	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *FullFilePath))
	{
		V_LOG("FFILEHELPER:>> Invalid File");
		return false;
		//~~
	}

	//Testing
	V_LOG("Loaded File Size");
	V_LOG(FString::FromInt(TheBinaryArray.Num()));

	//File Load Error
	if (TheBinaryArray.Num() <= 0) return false;

	//~
	//        Read the Data Retrieved by GFileManager
	//~

	
	FMemoryReader FromBinary = FMemoryReader(TheBinaryArray, true); //true, free data after done
	FromBinary.Seek(0);
	SaveLoadData(FromBinary, MapSector, SectorCoord);

	//~
	//                              Clean up 
	//~
	FromBinary.FlushCache();

	// Empty & Close Buffer 
	TheBinaryArray.Empty();
	FromBinary.Close();



	return true;
}

void AFactionsOfInadelPlayerController::SaveLoadData(FArchive& Ar, FMapSector& MapSector, FVector& SectorCoord)
{
	Ar << MapSector;
	Ar << SectorCoord;
}

void AFactionsOfInadelPlayerController::BeginPlay()
{
	UWorld* World = GetWorld();

	if (World)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(World, AGridSectorSetup::StaticClass(), FoundActors);
		SectorSetup = Cast<AGridSectorSetup>(FoundActors[0]);
	}
}
