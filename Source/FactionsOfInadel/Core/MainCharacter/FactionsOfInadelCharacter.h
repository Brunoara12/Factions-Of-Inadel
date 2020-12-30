// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/TimelineComponent.h"
#include "Blueprint/UserWidget.h"

#include "FactionsOfInadelCharacter.generated.h"

UCLASS(Blueprintable)
class AFactionsOfInadelCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFactionsOfInadelCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }


	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HelpWidgetClass;

	UPROPERTY(EditAnywhere)
	class UUserWidget* InfoWidget;



	UPROPERTY(EditAnywhere)
		UCurveFloat* SpeedCurve;

	UFUNCTION()
		void BeginMove();

	UFUNCTION()
		void FinishedMove();

	void InitiateMove();

	bool bMoving;
	bool bReadyState;
	float MoveValue;
	float CurveFloatValue;
	float TimelineValue;
	FTimeline MyTimeline;
	FVector TargetLocation;
	FVector TempCurrentLocation;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
};

