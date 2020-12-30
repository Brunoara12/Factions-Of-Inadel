// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Serialization/Archive.h"
#include "Serialization/BufferArchive.h"
#include "HAL/FileManager.h"

#include "../Maps/GridSetup.h"
#include "../Maps/GridSectorSetup.h"

#include "FactionsOfInadelPlayerController.generated.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FMapSector& MapSector)
{

	for (int32 l = 0; l < MapSector.Layers.Num(); l++)
	{
		for (int32 r = 0; r < MapSector.Layers[0].Rows.Num(); r++)
		{
			for (int32 c = 0; c < MapSector.Layers[0].Rows[0].Columns.Num(); c++)
			{
				Ar << MapSector.Layers[l].Rows[r].Columns[c];
			}
		}
	}

	return Ar;
}

UCLASS()
class AFactionsOfInadelPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFactionsOfInadelPlayerController();

	bool SaveGameDataToFile(const FString& FullFilePath, FBufferArchive& ToBinary, FMapSector& MapSector, FVector& SectorCoord);
	bool LoadGameDataFromFile(const FString& FullFilePath, FMapSector& MapSector, FVector& SectorCoord);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** True if the controlled character is in edit mode. */
	uint32 bInEditMode : 1;

	AGridSectorSetup* SectorSetup;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	/** Input handlers for left and right click*/
	void OnLeftClick();
	void OnRightClick();

	/** Input handlers for Save and Load map objects action.*/
	void OnSaveMapData();
	void OnLoadMapData();

	/** Input handlers for EditingMode */
	void OnEditMode();

	/** Input handlers for Movement */
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBack();

	//FArchive is shared base class for FBufferArchive and FMemoryReader
	void SaveLoadData(FArchive& Ar, FMapSector& MapSector, FVector& SectorCoord);

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};


