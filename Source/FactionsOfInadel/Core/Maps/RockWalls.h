// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridBase.h"
#include "RockWalls.generated.h"

/**
 *
 */
UCLASS()
class FACTIONSOFINADEL_API ARockWalls : public AGridBase
{
	GENERATED_BODY()

public:
	ARockWalls();

	virtual void BeginPlay() override;
};
