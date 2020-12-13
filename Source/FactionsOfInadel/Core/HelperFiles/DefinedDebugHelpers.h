// Fill out your copyright notice in the Description page of Project Settings.
/*
Originally called Joy String
Current Class, File, and Line Number!
by Rama (link: "https://wiki.unrealengine.com/Logs,_Printing_Class_Name,_Function_Name,_Line_Number_of_your_Calling_Code!")
appended by Bruno Rebaza

PreProcessor commands to get
a. Class name
b. Function Name
c. Line number
d. Function Signature (including parameters)

Gives you a UE4 FString anywhere in your code that these macros are used!

Ex:
You can use CURCLASS anywhere to get a UE4 FString back telling you
what the current class is where you called this macro!

Ex:
This macro prints the class and line along with the message of your choosing!
SCREENMSG("Have fun today!");

*/
#pragma once
#include "Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// ########################################################################################################
//Current Class Name + Function Name where this is called!
#define CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called!
#define CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called!
#define CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

//Current Line Number in the code where this is called!
#define CUR_LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define CUR_CLASS_LINE (CUR_CLASS + "(" + CUR_LINE + ")")

//Current Function Signature where this is called!
#define CUR_FUNCSIG (FString(__FUNCSIG__))

// ########################################################################################################
// Screen Message
/*
	Class Name + Line# + print a message to yourself!
	Ex:
	SCREENMSG("Debugging Here!");
*/
#define SCREENMSG(Param1) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Emerald, *(CUR_CLASS_LINE + ": " + Param1)) )

/*
	Class Name + Line# + Two Message Parameters!
	Ex:
	SCREENMSG2("TEST", "Debugging Here!");
*/
#define SCREENMSG2(Param1,Param2) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Emerald, *(CUR_CLASS_LINE + ": " + Param1 + " " + Param2)) )

/*
	Class Name + Line# + First Param is a message, second is a float!
	Ex:
	SCREENMSGF("TEST", 10.5);
*/
#define SCREENMSGF(Param1,Param2) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Emerald, *(CUR_CLASS_LINE + ": " + Param1 + " " + FString::SanitizeFloat(Param2))) )

/*
	Class Name + Line# + print a message to yourself with a color of your choosing!
	Ex:
	SCREENMSGC("Debugging Here!", Yellow);
*/
#define SCREENMSGC(Param1, ParamC) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::ParamC, *(CUR_CLASS_LINE + ": " + Param1)) )

/*
	Class Name + Line# + Two Message Parameters with a color of your choosing!
	Ex:
	SCREENMSG2C("TEST", "This should be Blue!", Blue);
*/
#define SCREENMSG2C(Param1,Param2,ParamC) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::ParamC, *(CUR_CLASS_LINE + ": " + Param1 + " " + Param2)) )

// type CURCLASS while debugging to if you hit that line of code.
#define CURCLASS SCREENMSG("We are in this class and Line#")

// ########################################################################################################
//UE LOG!
/*
	Class Name + Line# + print a message to yourself in the LogTemp Category!
	Ex:
	V_LOG("Debugging Here!");
*/
#define V_LOG(Param1) 		UE_LOG(LogTemp,Warning,TEXT("%s: %s"), *CUR_CLASS_LINE, *FString(Param1))

/*
	Class Name + Line# + print a message to yourself in the "LogCat" Category of your choosing!
	This will become more relevant when and if we create our own Log Categories
	Ex:
	V_LOGC(LogTemp, "Debugging Here on LogTemp!");
*/
#define V_LOGC(LogCat, Param1) 	UE_LOG(LogCat,Warning,TEXT("%s: %s"), *CUR_CLASS_LINE, *FString(Param1))

/*
	Class Name + Line# + First Param is a message, second is a float!
	Ex:
	V_LOGF("Debugging Here!", 10.5);
*/
#define V_LOGF(Param1, Float) 	UE_LOG(LogTemp,Warning,TEXT("%s: %s %f"), *CUR_CLASS_LINE, *FString(Param1), Float)

/*
	Class Name + Line# + First Param is a message, second is an int!
	Ex:
	int32 health = 95;
	V_LOGI("health: ", health);
*/
#define V_LOGI(Param1, Int) 	UE_LOG(LogTemp,Warning,TEXT("%s: %s %d"), *CUR_CLASS_LINE, *FString(Param1), Int)

/*
	Class Name + Line# + First Param is a message, second is a vector!
	Ex:
	FVector Location(95, 1, 32);
	V_LOGI("Location: ", Location);
*/
#define V_LOGVEC(Param1, Vec) 	UE_LOG(LogTemp,Warning,TEXT("%s: %s %s"), *CUR_CLASS_LINE, *FString(Param1), *Vec.ToString())

/*
	Class Name + Line# + First Param is a message, second is an object that has an FName!
	Ex:
	APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	V_LOGNAME("PC's Name: ", PC);
*/
#define V_LOGNAME(Param1, Object) UE_LOG(LogTemp,Warning,TEXT("%s: %s %s"), *CUR_CLASS_LINE, *FString(Param1), *Object->GetFName().ToString())

/*
	Class Name + Line# + First Param is a message, second is a bool!
	Ex:
	bool bCanJump = false;
	V_LOGB("bCanJump: ", bCanJump);
*/
#define V_LOGB(Param1, Bool) 	UE_LOG(LogTemp,Warning,TEXT("%s: %s %s"), *CUR_CLASS_LINE, *FString(Param1), (Bool ? TEXT("True"): TEXT("False")))

/*
	Class Name + Line# + arbitrary number of vars that you output, and of any type, similar to standrad UE_LOG functionality!
	Ex:
	int32 Health = 100;
	float ArmorPct = 52.33;
	FVector Location(33,12,1);

	V_LOGM("Health: %d, ArmorPct: %f, Loc: %s",  Health, ArmorPct, *Location.ToString());*/
#define V_LOGM(FormatString , ...) UE_LOG(LogTemp,Warning,TEXT("%s: %s"), 	*CUR_CLASS_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ ) )

	/*
		Class Name + Line# + print a message to yourself in the "LogCat" Category with "Verbosity" Level of your choosing!

		Fatal: Fatal level logs are always printed to console and log files and crashes even if logging is disabled.
		Error: Error level logs are printed to console and log files. These appear red by default.
		Warning: Warning level logs are printed to console and log files. These appear yellow by default.
		Display: Display level logs are printed to console and log files.
		Log: Log level logs are printed to log files but not to the in-game console. They can still be viewed in editor as they appear via the Output Log window.
		Verbose: Verbose level logs are printed to log files but not the in-game console. This is usually used for detailed logging and debugging.
		VeryVerbose: VeryVerbose level logs are printed to log files but not the in-game console. This is usually used for very detailed logging that would otherwise spam output.
		Ex:
		V_LOGV(LogTemp, Error, "User Accessed Private Stuff!");
	*/
#define V_LOGV(LogCat, Verbosity, Param1) UE_LOG(LogCat, Verbosity, TEXT("%s: %s"), 	*CUR_CLASS_LINE, *FString(Param1))


	// ########################################################################################################

	/*
		Draws a red point of size 15 for X second at location of your choosing!
		Ex:
		FVector Location(10,10,8);
		DEBUGP(Location);
	*/
#define DEBUGP(ParamL, ParamT) if(GetWorld()) DrawDebugPoint( 	GetWorld(), 	ParamL, 	12.5,	FColor::Red, 	false, ParamT);

	/*
		Draws a point of size 15 for X second at location and color of your choosing!
		Ex:
		FVector Location(10,10,8);
		DEBUGP(Location, Blue);
	*/
#define DEBUGPC(ParamL, ParamC, ParamT) if(GetWorld()) DrawDebugPoint( 	GetWorld(), 	ParamL, 	12.5,	FColor::ParamC, 	false, ParamT);

	/*
		Draws a Green line of thickness 10 for X second through Start location and End Location!
		Ex:
		FVector StartLocation(10,10,8);
		FVector EndLocation(10,10,55);
		DEBUGL(StartLocation, EndLocation);
	*/
#define DEBUGL(ParamS, ParamE, ParamT) if(GetWorld()) DrawDebugLine( 	GetWorld(), 	ParamS, 	ParamE, 	FColor::Green, 	false, ParamT, 0, 	2.5);

	/* 	Draws line of thickness 10 for X second through Start location and End Location and color of your choosing!
		Ex:
		FVector StartLocation(10,10,8);
		FVector EndLocation(10,10,55);
		DEBUGLC(StartLocation, EndLocation,Blue);
	*/
#define DEBUGLC(ParamS, ParamE, ParamC, ParamT) if(GetWorld()) DrawDebugLine( 	GetWorld(), 	ParamS, 	ParamE, 	FColor::ParamC, 	false, ParamT, 0, 	2.5);

	/*
		Draws a Green sphere at Location,radius of your choosing for X seconds!
		Ex:
		FVector CenterLocation(10,10,8);
		DEBUGS(CenterLocation, 1);
	*/
#define DEBUGS(ParamL, ParamR, ParamT) if(GetWorld()) DrawDebugSphere( 	GetWorld(), 	ParamL, 	ParamR, 	32, 	FColor::Green, false, ParamT,0 );

	/*
		Draws a sphere at Location,radius and color of your choosing for X seconds!
		Ex:
		FVector CenterLocation(10,10,8);
		DEBUGSC(CenterLocation, 1, Blue);
	*/
#define DEBUGSC(ParamL, ParamR, ParamC, ParamT) if(GetWorld()) DrawDebugSphere( 	GetWorld(), 	ParamL, 	ParamR, 	32, 	FColor::ParamC, false, ParamT,0 );

	/*
		This is for EnumString from value
		Ex:
		UENUM()
		enum ESampleColor
		{
			Yellow,
			Blue,
			Red
		}
		ESampleColor Colors;
		Colors = ESampleColor::Blue
		GETENUMSTRING("ESampleColor", Colors);  Output: FString("Blue")
	*/
#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetNameStringByIndex((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )



	/**
	 *
	 */
class DefinedDebugHelpers
{
public:
	DefinedDebugHelpers();
	~DefinedDebugHelpers();
};