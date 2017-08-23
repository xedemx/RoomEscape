// Copyright Adam Horvath-Voros 2017

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Gameframework/Pawn.h"
#include "Gameframework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner(); //Find the owning Actor

	if (!PressurePlate)//Protect pointer if none selected
	{
		UE_LOG(LogTemp, Error, TEXT("No Trigger Volume is setup for  %s !!"), *(GetOwner()->GetName()))
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();  //Broadcast this to Blueprint so we can handle the event there
	}
	else
	{
		OnClose.Broadcast();  //Broadcast this to Blueprint so we can handle the event there
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;

	//Find all the overlapping actors on the trigger volume
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//Debugging only
		//UE_LOG(LogTemp,Warning, TEXT("%s on pressure plate."), *(Actor->GetName()))
	}

	return TotalMass;
}