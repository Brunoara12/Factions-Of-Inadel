// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactionsOfInadelCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"

#include "../HelperFiles/DefinedDebugHelpers.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AFactionsOfInadelCharacter::AFactionsOfInadelCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, -90.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());



	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//CurrentSpeed = 600;
	bReadyState = true;
	bMoving = false;
}

void AFactionsOfInadelCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	MyTimeline.TickTimeline(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	InfoWidget->GetWidgetFromName("RightHelp")->SetVisibility(ESlateVisibility::Visible);
}

void AFactionsOfInadelCharacter::BeginMove()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = MoveValue * SpeedCurve->GetFloatValue(TimelineValue);

	 //NewLocation = FVector(GetActorLocation().X + CurveFloatValue, GetActorLocation().Y, GetActorLocation().Z);
	FVector NewLocation = FMath::Lerp(TempCurrentLocation, TargetLocation, CurveFloatValue);
	V_LOGM("location: %s", *NewLocation.ToString());

	SetActorLocation(NewLocation);

}

void AFactionsOfInadelCharacter::FinishedMove()
{
	bReadyState = true;
	bMoving = false;
}

void AFactionsOfInadelCharacter::InitiateMove()
{
	if (bReadyState)
	{
		TempCurrentLocation = GetActorLocation();
		TargetLocation = FVector(GetActorLocation().X + 100, GetActorLocation().Y, GetActorLocation().Z);
		MyTimeline.PlayFromStart();

	}
}

void AFactionsOfInadelCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveValue = 1.f;

	if (SpeedCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("BeginMove"));
		TimelineFinishedCallback.BindUFunction(this, FName("FinishedMove"));

		MyTimeline.AddInterpFloat(SpeedCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		MyTimeline.SetPlayRate(5);
		V_LOGF("",MyTimeline.GetPlayRate());
	}

	if (HelpWidgetClass)
	{
		InfoWidget = CreateWidget<UUserWidget>(GetWorld(), HelpWidgetClass);

		if (InfoWidget)
		{
			InfoWidget->AddToViewport();
		}
	}
}

void AFactionsOfInadelCharacter::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
	check(inputComponent);

	// Player Movement Keybind
	inputComponent->BindAction("Right", IE_Pressed, this, &AFactionsOfInadelCharacter::InitiateMove);

}
